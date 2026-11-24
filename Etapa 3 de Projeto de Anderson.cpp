#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

struct Conta {
    int numero;
    string titular;
    string cpf;
    int tipo;
    double saldo;
    int ativa; // 1 = ativa, 0 = inativa
};

struct Movimentacao {
    int numeroConta;
    char tipo; // 'D' ou 'S'
    double valor;
};

vector<Conta> contas;
vector<Movimentacao> movimentacoes;

// --- FUNÇÕES DE ARQUIVO ---

void carregarContas() {
    ifstream arquivo("contas.txt");
    if (!arquivo.is_open()) return; // Se não existir, inicia vazio[cite: 6]

    string linha;
    while (getline(arquivo, linha)) {
        stringstream ss(linha);
        string segmento;
        Conta c;

        getline(ss, segmento, ';'); c.numero = stoi(segmento);
        getline(ss, c.titular, ';');
        getline(ss, c.cpf, ';');
        getline(ss, segmento, ';'); c.tipo = stoi(segmento);
        getline(ss, segmento, ';'); c.saldo = stod(segmento);
        getline(ss, segmento, ';'); c.ativa = stoi(segmento);

        contas.push_back(c);
    }
    arquivo.close();
}

void salvarContas() {
    ofstream arquivo("contas.txt");
    if (arquivo.is_open()) {
        for (size_t i = 0; i < contas.size(); i++) {
            arquivo << contas[i].numero << ";"
                    << contas[i].titular << ";"
                    << contas[i].cpf << ";"
                    << contas[i].tipo << ";"
                    << contas[i].saldo << ";"
                    << contas[i].ativa << endl;
        }
        arquivo.close();
    }
}

void carregarMovimentacoes() {
    ifstream arquivo("movimentacoes.txt");
    if (!arquivo.is_open()) return;

    string linha;
    while (getline(arquivo, linha)) {
        stringstream ss(linha);
        string segmento;
        Movimentacao m;

        getline(ss, segmento, ';'); m.numeroConta = stoi(segmento);
        getline(ss, segmento, ';'); m.tipo = segmento[0];
        getline(ss, segmento, ';'); m.valor = stod(segmento);

        movimentacoes.push_back(m);
    }
    arquivo.close();
}

void salvarMovimentacoes() {
    ofstream arquivo("movimentacoes.txt");
    if (arquivo.is_open()) {
        for (size_t i = 0; i < movimentacoes.size(); i++) {
            arquivo << movimentacoes[i].numeroConta << ";"
                    << movimentacoes[i].tipo << ";"
                    << movimentacoes[i].valor << endl;
        }
        arquivo.close();
    }
}

// --- FUNÇÕES AUXILIARES ---

int buscarConta(int numero) {
    for (size_t i = 0; i < contas.size(); i++) {
        if (contas[i].numero == numero && contas[i].ativa == 1) {
            return i;
        }
    }
    return -1;
}

// --- PROGRAMA PRINCIPAL ---

int main() {
    // Carrega os dados salvos ao iniciar
    carregarContas();
    carregarMovimentacoes();

    int opcao;
    do {
        cout << "\n=== SISTEMA BANCARIO (ETAPA 3) ===\n";
        cout << "1. Cadastrar Conta\n";
        cout << "2. Listar Contas\n";
        cout << "3. Consultar Saldo\n";
        cout << "4. Depositar\n";
        cout << "5. Sacar\n";
        cout << "6. Transferir\n";
        cout << "7. Exibir Extrato\n";
        cout << "0. Sair\n";
        cout << "Escolha uma opcao: ";
        cin >> opcao;

        if (opcao == 1) {
            Conta c;
            cout << "Numero da conta: ";
            cin >> c.numero;
            if (buscarConta(c.numero) != -1) {
                cout << "[Erro] Conta ja existe!\n";
                continue;
            }
            cout << "Nome do cliente: ";
            cin.ignore();
            getline(cin, c.titular);
            cout << "CPF: ";
            cin >> c.cpf;
            cout << "Tipo da conta (1-Corrente, 2-Poupanca): ";
            cin >> c.tipo;
            c.saldo = 0.0;
            c.ativa = 1;

            contas.push_back(c);
            salvarContas(); // Salva permanentemente[cite: 6]
            cout << "[Sucesso] Conta cadastrada e salva!\n";

        } else if (opcao == 2) {
            cout << "\n--- CONTAS CADASTRADAS ---\n";
            for (size_t i = 0; i < contas.size(); i++) {
                if (contas[i].ativa == 1) {
                    cout << "Conta: " << contas[i].numero << " | Titular: " << contas[i].titular << " | Saldo: R$ " << contas[i].saldo << endl;
                }
            }

        } else if (opcao == 3) {
            int num;
            cout << "Numero da conta: ";
            cin >> num;
            int idx = buscarConta(num);
            if (idx != -1) {
                cout << "Titular: " << contas[idx].titular << "\nSaldo: R$ " << contas[idx].saldo << endl;
            } else {
                cout << "[Erro] Conta nao encontrada!\n";
            }

        } else if (opcao == 4) {
            int num;
            double val;
            cout << "Numero da conta: ";
            cin >> num;
            int idx = buscarConta(num);
            if (idx != -1) {
                cout << "Valor do deposito: R$ ";
                cin >> val;
                if (val > 0) {
                    contas[idx].saldo += val;
                    movimentacoes.push_back({num, 'D', val});
                    salvarContas();
                    salvarMovimentacoes();
                    cout << "[Sucesso] Deposito realizado e salvo!\n";
                } else {
                    cout << "[Erro] Valor invalido!\n";
                }
            } else {
                cout << "[Erro] Conta nao encontrada!\n";
            }

        } else if (opcao == 5) {
            int num;
            double val;
            cout << "Numero da conta: ";
            cin >> num;
            int idx = buscarConta(num);
            if (idx != -1) {
                cout << "Valor do saque: R$ ";
                cin >> val;
                if (val > 0 && val <= contas[idx].saldo) {
                    contas[idx].saldo -= val;
                    movimentacoes.push_back({num, 'S', val});
                    salvarContas();
                    salvarMovimentacoes();
                    cout << "[Sucesso] Saque realizado e salvo!\n";
                } else {
                    cout << "[Erro] Saldo insuficiente ou valor invalido!\n";
                }
            } else {
                cout << "[Erro] Conta nao encontrada!\n";
            }

        } else if (opcao == 6) {
            int orig, dest;
            double val;
            cout << "Conta de origem: ";
            cin >> orig;
            cout << "Conta de destino: ";
            cin >> dest;

            int iOrig = buscarConta(orig);
            int iDest = buscarConta(dest);

            if (iOrig != -1 && iDest != -1) {
                cout << "Valor da transferencia: R$ ";
                cin >> val;
                if (val > 0 && val <= contas[iOrig].saldo) {
                    contas[iOrig].saldo -= val;
                    contas[iDest].saldo += val;

                    // Registra a saída na origem e a entrada no destino[cite: 5]
                    movimentacoes.push_back({orig, 'S', val});
                    movimentacoes.push_back({dest, 'D', val});

                    salvarContas();
                    salvarMovimentacoes();
                    cout << "[Sucesso] Transferencia realizada e salva!\n";
                } else {
                    cout << "[Erro] Saldo insuficiente ou valor invalido!\n";
                }
            } else {
                cout << "[Erro] Conta de origem ou destino nao encontrada!\n";
            }

        } else if (opcao == 7) {
            int num;
            cout << "Numero da conta para o extrato: ";
            cin >> num;
            cout << "\n--- EXTRATO DA CONTA " << num << " ---\n";
            for (size_t i = 0; i < movimentacoes.size(); i++) {
                if (movimentacoes[i].numeroConta == num) {
                    cout << "Tipo: " << movimentacoes[i].tipo << " | Valor: R$ " << movimentacoes[i].valor << endl;
                }
            }
        }

    } while (opcao != 0);

    return 0;
}