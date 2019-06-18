/*
    jovita.cpp

    um programa desenvolvido pra substituir o porcesso
    manual de montagem de razonetes e balancetes referente
    a materia do 2o bimestre do 3o ano de gestao de negocios

    desenvolvido por Lucca Bibar, um desocupado no trabalho
    @bibaromito

*/

#include<math.h>
#include<iostream>
using std::cin;
using std::cout;
using std::endl;

#include <iomanip>
using std::fixed;
using std::setprecision;

#include<vector>
using std::vector;

#include<string>
using std::string;
using std::getline;

#ifdef _WIN32
    const string terminal = "cmd";
#else
    const string terminal = "bash";
#endif

struct tabela{
    string nome;
    vector<float> debita;
    vector<float> credita;
};

struct balancLin{
    string nome;
    float debMov;
    float credMov;
    float debSald;
    float credSald;
};

//suporte para cmd/bash
void clear(){
    if(terminal == "cmd"){
        system("cls");

    }
    else{
        system("clear");

    }
    
}

void clearbuff(){
    int c;
    do {
        c = getchar();

    } while (c != '\n' && c != EOF);

}

//le onde o razoente debita/credita, a partir de um preset de tables e permitindo criar mais
void escolheTabela(vector<tabela> &tabelas, string acao, float valor){
    int inp;

    cout << endl << "selecione a tabela em que o razonete " << acao << ":" << endl;
    for(int i = 0; i != tabelas.size(); i++){
        cout << i << " - " << tabelas[i].nome << endl;

    }
    cout << tabelas.size() << " - " << "[adicionar nova tabela]" << endl;

    do{
        clearbuff();
        cout << "> ";
        cin >> inp;

    }while(inp < 0 || inp > tabelas.size());

    //cria nova tabela, caso requisitado
    if(inp == tabelas.size()){
        cout << endl << "inisra o nome da nova tabela: " << endl;
        tabela tabelaNova;
        cout << "> ";
        clearbuff();
        getline(cin, tabelaNova.nome);
        tabelas.push_back(tabelaNova);

    }

    //registra o deb/cred na tabela
    if(acao == "credita"){
        tabelas[inp].credita.push_back(valor);

    }
    else if(acao == "debita"){
        tabelas[inp].debita.push_back(valor);

    }
    

}

//le o valor do razoente e valida
float readValor(){
    cout << endl << "insira o valor positivo do razonete [ou 0 para encerrar]: ";
    float inp;
    do{
        cin >> inp;
        
    }while(inp < 0);
    return inp;
}

//retorna o tamanho de um float, sem contar dps do ponto
int floatSize(float num){
    if(num < 10){
        return 1;
    }
    
    int cont = 1;
    do{
        num /= 10;
        cont++;

    }while(num > 10);
    
    return cont;
}

//compara dois floats e retorna o numero de casas antes da virgula do maior
int maiorFloatSize(float a, float b){
    return (floatSize(a) >= floatSize(b)) ? floatSize(a) : floatSize(b);

}

//imprimum numero n de caracteres c;
void printCharstream(int n, char c = ' '){
    for (int h = 0; h < n; h++){
        cout << c;
        
    }
    
}

int main(){
    cout << fixed;
    cout << setprecision(2);

    vector<tabela> tabelas;
    tabela novaTabela;
    novaTabela.nome = "captal inicial";
    tabelas.push_back(novaTabela);
    novaTabela.nome = "caixa";
    tabelas.push_back(novaTabela);

    //leitura dos razoentes
    float valorRead;
    string credRead;
    string debRead;
    while(true){
        try{
            clear();
            valorRead = readValor();
            if(valorRead == 0){
                break;

            }

            escolheTabela(tabelas, "credita", valorRead);
            escolheTabela(tabelas, "debita", valorRead);
            
        }
        catch(...){
            continue;

        }

    }

    //monta as tabelinhas ja formatadas
    clear();
    for(int j = 0; j < tabelas.size(); j++){
        //define o tamanho do maior numero de credito
        int credTam = 0;
        for (int k = 0; k != tabelas[j].credita.size(); k++){
            int tam = floatSize(tabelas[j].credita[k]);
            credTam = (credTam >= tam) ? credTam : tam;
            
        }
        credTam += 3;
        
        //define o tamanho do maior numero de debito
        int debTam = 0;
        for (int l = 0; l != tabelas[j].debita.size(); l++){
            int tam = floatSize(tabelas[j].debita[l]);
            debTam = (debTam >= tam) ? debTam : tam;
            
        }
        debTam += 3;

        //define qunats linhas a tabela vai ter
        int lin = tabelas[j].debita.size();
        lin = (lin >= tabelas[j].credita.size()) ? lin : tabelas[j].credita.size();
        
        //nome da tabela
        cout << endl << " " << tabelas[j].nome;

        //traco horizontal
        cout << endl << " ";
        for(int m = 0; m != (debTam + credTam + 3); m++){
            cout << "_";

        }

        //valores
        for (int n = 0; n < lin; n++){
            cout << endl << " ";

            //debita
            if(n < tabelas[j].debita.size()){
                printCharstream(debTam - floatSize(tabelas[j].debita[n]) - 3);
                cout << tabelas[j].debita[n];
                
            }
            else{
                printCharstream(debTam);
            
            }
            
            
            //traco vertcal
            cout << " | ";

            //credita
            if(n < tabelas[j].credita.size()){
                printCharstream(credTam - floatSize(tabelas[j].credita[n]) - 3);
                cout << tabelas[j].credita[n];
            
            }
            else{
                printCharstream(credTam);
            
            }
        
        }
        cout << endl << endl;

    }
    
    //monta o balancete
    int nomeTam = 0;
    int movTam = 7; //"credito".length()
    int saldTam = 7; //"credito".length()
    vector<balancLin> linhas;
    balancLin total;
    total.nome = "total";
    total.credMov = 0;
    total.debMov = 0;
    total.credSald = 0;
    total.debSald = 0;
    while(tabelas.size() != 0){
        //monta uma linha do balancete
        balancLin novaLin;
        novaLin.nome = tabelas[0].nome;
        novaLin.credMov = 0;
        novaLin.debMov = 0;

        int sumIter = tabelas[0].debita.size();
        sumIter = (sumIter >= tabelas[0].credita.size()) ? sumIter : tabelas[0].credita.size();
        for (int q = 0; q < sumIter; q++){
            novaLin.credMov += (q < tabelas[0].credita.size()) ? tabelas[0].credita[q] : 0; 
            novaLin.debMov += (q < tabelas[0].debita.size()) ? tabelas[0].debita[q] : 0; 

        }

        float novoSaldo = novaLin.credMov - novaLin.debMov;
        novaLin.credSald = (novoSaldo > 0) ? novoSaldo : 0;
        novaLin.debSald = (novoSaldo < 0) ? novoSaldo * -1 : 0;
        
        //soma tudo ao total
        total.credMov += novaLin.credMov;
        total.debMov += novaLin.debMov;
        total.credSald += novaLin.credSald;
        total.debSald += novaLin.debSald;

        //calcula o tamanho de tudo
        nomeTam = (nomeTam >= novaLin.nome.length()) ? nomeTam : novaLin.nome.length();
        movTam = maiorFloatSize(movTam, novaLin.credMov);
        movTam = maiorFloatSize(movTam, novaLin.debMov);
        saldTam = maiorFloatSize(saldTam, novaLin.credSald);
        saldTam = maiorFloatSize(saldTam, novaLin.debSald);

        //add pro vetor que monta  a tabela
        linhas.push_back(novaLin);

        //apaga o indice que ele acacbo de ler
        tabelas.erase(tabelas.begin());
    }
    //compara o tamanho de tudo com o total
    movTam = maiorFloatSize(movTam, total.credMov);
    movTam = maiorFloatSize(movTam, total.debMov);
    saldTam = maiorFloatSize(saldTam, total.credSald);
    saldTam = maiorFloatSize(saldTam, total.debSald);
    //uns ajustes finais no tamanho
    movTam += 3;
    saldTam += 3;
    int tamTotal = nomeTam + (movTam * 2) + (saldTam * 2) + 16; //os 16 eh por causa do  monte de espaco em branco e tracos etc 
    
    //imprime o balancete
    //header
    printCharstream(tamTotal, '_');
    cout << endl;

    cout << "| " << "nome";
    printCharstream(nomeTam - 4);
    cout << " ";

    cout << "| " << "movimento";
    printCharstream((movTam * 2) + 3 - 9);
    cout << " ";

    cout << "| " << "saldo";
    printCharstream((saldTam * 2) + 3 - 5);
    cout << " |" << endl;

    //subheader
    printCharstream(tamTotal, '_');
    cout << endl;

    cout << "| ";
    printCharstream(nomeTam);
    cout << " ";

    for (int o = 0; o != 2; o++){
        cout << "| " << "debito";
        printCharstream(movTam - 6);
        cout << " ";

        cout << "| " << "credito";
        printCharstream(saldTam - 7);
        cout << " ";
        
    }
    cout << "|" << endl;

    //linhas da tabela
    printCharstream(tamTotal, '_');
    cout << endl;

    while(linhas.size() != 0){
        cout << "| " << linhas[0].nome;
        printCharstream(nomeTam - linhas[0].nome.length());
        cout << " ";

        cout << "| ";
        printCharstream(movTam - (floatSize(linhas[0].debMov) + 3));
        cout << linhas[0].debMov << " ";

        cout << "| ";
        printCharstream(movTam - (floatSize(linhas[0].credMov) + 3));
        cout << linhas[0].credMov << " ";

        cout << "| ";
        printCharstream(saldTam - (floatSize(linhas[0].debSald) + 3));
        cout << linhas[0].debSald << " ";

        cout << "| ";
        printCharstream(saldTam - (floatSize(linhas[0].credSald) + 3));
        cout << linhas[0].credSald << " |" << endl;
        
        linhas.erase(linhas.begin());
    }

    //linha total
    printCharstream(tamTotal, '_');
    cout << endl;

    cout << "| " << total.nome;
    printCharstream(nomeTam - total.nome.length());
    cout << " ";

    cout << "| ";
    printCharstream(movTam - (floatSize(total.debMov) + 3));
    cout << total.debMov << " ";

    cout << "| ";
    printCharstream(movTam - (floatSize(total.credMov) + 3));
    cout << total.credMov << " ";

    cout << "| ";
    printCharstream(saldTam - (floatSize(total.debSald) + 3));
    cout << total.debSald << " ";

    cout << "| ";
    printCharstream(saldTam - (floatSize(total.credSald) + 3));
    cout << total.credSald << " |" << endl;

    return 0;
}