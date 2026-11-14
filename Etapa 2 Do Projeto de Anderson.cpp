#include <iostream>
#include <vector>
#include <string>
#include <iomanip> // Para formatar os valores monetários com 2 casas decimais

using namespace std;

// Estrutura que representa uma Conta Bancária completa
struct Conta {
    int numero;
    string titular;
    string cpf;
    double saldo;
    vector<string> extrato; // Guarda o histórico de movimentações da conta
};

// --- FUNÇÕES AUXILIARES ---

// Busca uma conta pelo número e retorna sua posição no vetor (ou -1 se não existir)
int buscarConta(const vector<Conta>& contas, int numero) {
    for (size_t i = 0; i < contas.size(); i++) {
        if (contas[i].numero == numero) {
            return i;
        }
    }
    return -1;
}

// --- OPERAÇÕES DO SISTEMA ---

void criarConta(vector<Conta>& contas) {
    Conta nova;
    cout << "\n--- ABRIR NOVA CONTA ---\n";
    cout << "Numero da conta: ";
    cin >> nova.numero;

    if (buscarConta(contas, nova.numero) != -1) {
        cout << "[Erro] Ja existe uma conta cadastrada com esse numero!\n";
        return;
    }

    cin.ignore();
    cout << "Nome completo do titular: ";
    getline(cin, nova.titular);

    cout << "CPF do titular: ";
    getline(cin, nova.cpf);

    nova.saldo = 0.0;
    nova.extrato.push_back("Conta aberta com saldo inicial de R$ 0.00");

    contas.push_back(nova);
    cout << "[Sucesso] Conta criada para " << nova.titular << "!\n";
}

void consultarDados(const vector<Conta>& contas) {
    int num;
    cout << "\n--- CONSULTA DE CONTA ---\n";
    cout << "Digite o numero da conta: ";
    cin >> num;

    int idx = buscarConta(contas, num);
    if (idx != -1) {
        cout << "Titular: " << contas[idx].titular << "\n";
        cout << "CPF: " << contas[idx].cpf << "\n";
        cout << fixed << setprecision(2);
        cout << "Saldo atual: R$ " << contas[idx].saldo << "\n";
    } else {
        cout << "[Erro] Conta nao encontrada!\n";
    }
}

void depositar(vector<Conta>& contas) {
    int num;
    double valor;
    cout << "\n--- DEPOSITO ---\n";
    cout << "Numero da conta: ";
    cin >> num;

    int idx = buscarConta(contas, num);
    if (idx != -1) {
        cout << "Valor do deposito: R$ ";
        cin >> valor;

        if (valor > 0) {
            contas[idx].saldo += valor;
            contas[idx].extrato.push_back("Deposito: +R$ " + to_string(valor).substr(0, to_string(valor).find(".") + 3));
            cout << "[Sucesso] Deposito realizado!\n";
        } else {
            cout << "[Erro] O valor deve ser maior que zero!\n";
        }
    } else {
        cout << "[Erro] Conta nao encontrada!\n";
    }
}

void sacar(vector<Conta>& contas) {
    int num;
    double valor;
    cout << "\n--- SAQUE ---\n";
    cout << "Numero da conta: ";
    cin >> num;

    int idx = buscarConta(contas, num);
    if (idx != -1) {
        cout << "Valor do saque: R$ ";
        cin >> valor;

        if (valor > 0 && valor <= contas[idx].saldo) {
            contas[idx].saldo -= valor;
            contas[idx].extrato.push_back("Saque: -R$ " + to_string(valor).substr(0, to_string(valor).find(".") + 3));
            cout << "[Sucesso] Saque realizado!\n";
        } else {
            cout << "[Erro] Saldo insuficiente ou valor invalido!\n";
        }
    } else {
        cout << "[Erro] Conta nao encontrada!\n";
    }
}

void transferir(vector<Conta>& contas) {
    int numOrigem, numDestino;
    double valor;
    cout << "\n--- TRANSFERENCIA ---\n";
    cout << "Numero da conta de origem: ";
    cin >> numOrigem;
    cout << "Numero da conta de destino: ";
    cin >> numDestino;

    if (numOrigem == numDestino) {
        cout << "[Erro] A conta de origem e destino nao podem ser iguais!\n";
        return;
    }

    int idxOrigem = buscarConta(contas, numOrigem);
    int idxDestino = buscarConta(contas, numDestino);

    if (idxOrigem != -1 && idxDestino != -1) {
        cout << "Valor da transferencia: R$ ";
        cin >> valor;

        if (valor > 0 && valor <= contas[idxOrigem].saldo) {
            contas[idxOrigem].saldo -= valor;
            contas[idxDestino].saldo += valor;

            string valorStr = to_string(valor).substr(0, to_string(valor).find(".") + 3);
            contas[idxOrigem].extrato.push_back("Transferencia enviada (Conta " + to_string(numDestino) + "): -R$ " + valorStr);
            contas[idxDestino].extrato.push_back("Transferencia recebida (Conta " + to_string(numOrigem) + "): +R$ " + valorStr);

            cout << "[Sucesso] Transferencia concluida!\n";
        } else {
            cout << "[Erro] Saldo insuficiente ou valor invalido!\n";
        }
    } else {
        cout << "[Erro] Uma ou ambas as contas nao foram encontradas!\n";
    }
}

void exibirExtrato(const vector<Conta>& contas) {
    int num;
    cout << "\n--- EXTRATO BANCARIO ---\n";
    cout << "Numero da conta: ";
    cin >> num;

    int idx = buscarConta(contas, num);
    if (idx != -1) {
        cout << "\nExtrato de " << contas[idx].titular << " (Conta " << num << "):\n";
        cout << "----------------------------------------\n";
        for (size_t i = 0; i < contas[idx].extrato.size(); i++) {
            cout << "- " << contas[idx].extrato[i] << "\n";
        }
        cout << "----------------------------------------\n";
        cout << fixed << setprecision(2);
        cout << "SALDO ATUAL: R$ " << contas[idx].saldo << "\n";
    } else {
        cout << "[Erro] Conta nao encontrada!\n";
    }
}

void listarTodasContas(const vector<Conta>& contas) {
    cout << "\n--- LISTA DE TODAS AS CONTAS ---\n";
    if (contas.empty()) {
        cout << "Nenhuma conta cadastrada no sistema.\n";
        return;
    }

    cout << fixed << setprecision(2);
    for (size_t i = 0; i < contas.size(); i++) {
        cout << "Conta: " << contas[i].numero
             << " | Titular: " << contas[i].titular
             << " | Saldo: R$ " << contas[i].saldo << "\n";
    }
}

void encerrarConta(vector<Conta>& contas) {
    int num;
    cout << "\n--- ENCERRAR CONTA ---\n";
    cout << "Numero da conta a ser encerrada: ";
    cin >> num;

    int idx = buscarConta(contas, num);
    if (idx != -1) {
        if (contas[idx].saldo > 0) {
            cout << "[Erro] Nao e possivel encerrar a conta com saldo positivo! Saque o valor antes.\n";
        } else {
            contas.erase(contas.begin() + idx);
            cout << "[Sucesso] Conta encerrada e removida do sistema!\n";
        }
    } else {
        cout << "[Erro] Conta nao encontrada!\n";
    }
}

// --- PROGRAMA PRINCIPAL ---

int main() {
    vector<Conta> contas;
    int opcao;

    do {
        cout << "\n========================================\n";
        cout << "      SISTEMA BANCARIO - ETAPA 2        \n";
        cout << "========================================\n";
        cout << "1. Criar Nova Conta\n";
        cout << "2. Consultar Dados e Saldo\n";
        cout << "3. Depositar\n";
        cout << "4. Sacar\n";
        cout << "5. Transferir\n";
        cout << "6. Ver Extrato (Historico)\n";
        cout << "7. Listar Todas as Contas\n";
        cout << "8. Encerrar Conta\n";
        cout << "0. Sair\n";
        cout << "========================================\n";
        cout << "Escolha uma opcao: ";
        cin >> opcao;

        switch (opcao) {
            case 1: criarConta(contas); break;
            case 2: consultarDados(contas); break;
            case 3: depositar(contas); break;
            case 4: sacar(contas); break;
            case 5: transferir(contas); break;
            case 6: exibirExtrato(contas); break;
            case 7: listarTodasContas(contas); break;
            case 8: encerrarConta(contas); break;
            case 0: cout << "Encerrando o sistema...\n"; break;
            default: cout << "[Erro] Opcao invalida! Tente novamente.\n";
        }

    } while (opcao != 0);

    return 0;
}