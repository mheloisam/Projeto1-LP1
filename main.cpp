#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>

class Astronauta
{
public:
    std::string cpf;
    std::string nome;
    int idade;
    bool disponivel;
    bool morto;
    std::vector<int> voosParticipados;

    Astronauta(std::string cpf, std::string nome, int idade)
        : cpf(cpf), nome(nome), idade(idade), disponivel(true), morto(false) {}
};

class Voo
{
public:
    int codigo;
    std::vector<Astronauta *> passageiros;
    enum Estado
    {
        PLANEJADO,
        EM_CURSO,
        FINALIZADO
    } estado;
    bool sucesso;

    Voo(int codigo) : codigo(codigo), estado(PLANEJADO), sucesso(false) {}

    void adicionarAstronauta(Astronauta *astronauta)
    {
        if (estado == PLANEJADO && astronauta->disponivel)
        {
            passageiros.push_back(astronauta);
        }
        else
        {
            std::cout << "\nAstronauta indisponivel.\n";
        }
    }

    void removerAstronauta(const std::string &cpf)
    {
        if (estado == PLANEJADO)
        {
            auto it = std::remove_if(passageiros.begin(), passageiros.end(),
                                     [&cpf](Astronauta *a)
                                     { return a->cpf == cpf; });
            passageiros.erase(it, passageiros.end());
        }
        else if (estado == EM_CURSO)
        {
            std::cout << "Esse voo ja foi lancado.\n";
        }
        else if (estado == FINALIZADO)
        {
            std::cout << "Esse voo ja foi finalizado.\n";
        }
    }

    void lancar()
    {
        if (estado == PLANEJADO && passageiros.size() > 0)
        {
            for (Astronauta *a : passageiros)
            {
                a->disponivel = false;
                a->voosParticipados.push_back(codigo);
            }
            estado = EM_CURSO;
        }
        else if (estado == FINALIZADO)
        {
            std::cout << "Esse voo ja foi finalizado.\n";
        }
    }

    void explodir()
    {
        if (estado == EM_CURSO)
        {
            for (Astronauta *a : passageiros)
            {
                a->morto = true;
            }
            estado = FINALIZADO;
            sucesso = false;
        }
        else if (estado == PLANEJADO)
        {
            std::cout << "Esse voo ainda nao foi lancado.\n";
        }
        else if (estado == FINALIZADO)
        {
            std::cout << "Esse voo ja foi finalizado.\n";
        }
    }

    void finalizar()
    {
        if (estado == EM_CURSO)
        {
            for (Astronauta *a : passageiros)
            {
                a->disponivel = true;
            }
            estado = FINALIZADO;
            sucesso = true;
        }
        else if (estado == PLANEJADO)
        {
            std::cout << "Esse voo ainda nao foi lancado\n";
        }
        else if (estado == FINALIZADO)
        {
            std::cout << "Esse voo ja foi finalizado.\n";
        }
    }
};

class Sistema
{
public:
    std::unordered_map<std::string, Astronauta *> astronautas;
    std::unordered_map<int, Voo *> voos;

    ~Sistema()
    {
        for (auto &kv : astronautas)
        {
            delete kv.second;
        }
        for (auto &kv : voos)
        {
            delete kv.second;
        }
    }

    void cadastrarAstronauta(std::string cpf, std::string nome, int idade)
    {
        astronautas[cpf] = new Astronauta(cpf, nome, idade);
        std::cout << "Astronauta cadastrado.\n";
    }

    void cadastrarVoo(int codigo)
    {
        voos[codigo] = new Voo(codigo);
        std::cout << "Voo cadastrado.\n";
    }

    void adicionarAstronautaEmVoo(int codigo, std::string cpf)
    {
        if (voos.count(codigo))
        {
            if (astronautas.count(cpf))
            {
                voos[codigo]->adicionarAstronauta(astronautas[cpf]);
                std::cout << "Astronauta adicionado.\n";
            }
            else
            {
                std::cout << "Astronauta com CPF " << cpf << " nao encontrado.\n";
            }
        }
        else
        {
            std::cout << "Voo com codigo " << codigo << " nao encontrado.\n";
        }
    }

    void removerAstronautaDeVoo(int codigo, std::string cpf)
    {
        if (voos.count(codigo))
        {
            voos[codigo]->removerAstronauta(cpf);
        }
        else
        {
            std::cout << "Voo com codigo " << codigo << " nao encontrado.\n";
        }
    }

    void lancarVoo(int codigo)
    {
        if (voos.count(codigo))
        {
            Voo *voo = voos[codigo];
            bool todosDisponiveis = std::all_of(voo->passageiros.begin(), voo->passageiros.end(),
                                                [](Astronauta *a)
                                                { return a->disponivel; });
            if (voo->passageiros.size() > 0 && todosDisponiveis)
            {
                voo->lancar();
                std::cout << "Voo lancado com sucesso!\n";
            }
            else if(voo->passageiros.size() <= 0)
            {
                std::cout << "Nao foi possivel lancar o voo, pois ele nao possui astronautas.\n";
            }
        }
        else
        {
            std::cout << "Voo com codigo " << codigo << " nao encontrado.\n";
        }
    }

    void explodirVoo(int codigo)
    {
        if (voos.count(codigo))
        {
            voos[codigo]->explodir();
        }
        else
        {
            std::cout << "Voo com codigo " << codigo << " nao encontrado.\n";
        }
    }

    void finalizarVoo(int codigo)
    {
        if (voos.count(codigo))
        {
            voos[codigo]->finalizar();
        }
        else
        {
            std::cout << "Voo com codigo " << codigo << " nao encontrado.\n";
        }
    }

    void listarVoos()
    {
        for (const auto &kv : voos)
        {
            Voo *voo = kv.second;
            std::cout << "Voo " << voo->codigo << " - ";
            switch (voo->estado)
            {
            case Voo::PLANEJADO:
                std::cout << "Planejado";
                break;
            case Voo::EM_CURSO:
                std::cout << "Em curso";
                break;
            case Voo::FINALIZADO:
                std::cout << "Finalizado" << (voo->sucesso ? " com sucesso" : " com falha");
                break;
            }
            std::cout << " - Passageiros: ";
            for (Astronauta *a : voo->passageiros)
            {
                std::cout << a->nome << " ";
            }
            std::cout << std::endl;
        }
    }

    void listarAstronautasMortos()
    {
        for (const auto &kv : astronautas)
        {
            Astronauta *a = kv.second;
            if (a->morto)
            {
                std::cout << "CPF: " << a->cpf << ", Nome: " << a->nome << ", Voos: ";
                for (int codigo : a->voosParticipados)
                {
                    std::cout << codigo << " ";
                }
                std::cout << std::endl;
            }
        }
    }
};

int main()
{
    Sistema sistema;
    int opcao;
    do
    {
        std::cout << "\nMenu:\n";
        std::cout << "1. Cadastrar astronauta\n";
        std::cout << "2. Cadastrar voo\n";
        std::cout << "3. Adicionar astronauta em voo\n";
        std::cout << "4. Remover astronauta de voo\n";
        std::cout << "5. Lancar voo\n";
        std::cout << "6. Explodir voo\n";
        std::cout << "7. Finalizar voo\n";
        std::cout << "8. Listar voos\n";
        std::cout << "9. Listar astronautas mortos\n";
        std::cout << "0. Sair\n";
        std::cout << "Escolha uma opcao: ";
        std::cin >> opcao;
        std::cout << "\n";

        switch (opcao)
        {
        case 1:
        {
            std::string cpf, nome;
            int idade;
            std::cout << "CPF: ";
            
            std::cin >> cpf;
            std::cout << "Nome: ";
            std::cin.ignore();
            std::getline(std::cin, nome);
            std::cout << "Idade: ";
            std::cin >> idade;
            sistema.cadastrarAstronauta(cpf, nome, idade);
            break;
        }
        case 2:
        {
            int codigo;
            std::cout << "Codigo do voo: ";
            std::cin >> codigo;
            sistema.cadastrarVoo(codigo);
            break;
        }
        case 3:
        {
            int codigo;
            std::string cpf;
            std::cout << "Codigo do voo: ";
            std::cin >> codigo;
            std::cout << "CPF do astronauta: ";
            std::cin >> cpf;
            sistema.adicionarAstronautaEmVoo(codigo, cpf);
            break;
        }
        case 4:
        {
            int codigo;
            std::string cpf;
            std::cout << "Codigo do voo: ";
            std::cin >> codigo;
            std::cout << "CPF do astronauta: ";
            std::cin >> cpf;
            sistema.removerAstronautaDeVoo(codigo, cpf);
            break;
        }
        case 5:
        {
            int codigo;
            std::cout << "Codigo do voo: ";
            std::cin >> codigo;
            sistema.lancarVoo(codigo);
            break;
        }
        case 6:
        {
            int codigo;
            std::cout << "Codigo do voo: ";
            std::cin >> codigo;
            sistema.explodirVoo(codigo);
            break;
        }
        case 7:
        {
            int codigo;
            std::cout << "Codigo do voo: ";
            std::cin >> codigo;
            sistema.finalizarVoo(codigo);
            break;
        }
        case 8:
        {
            sistema.listarVoos();
            break;
        }
        case 9:
        {
            sistema.listarAstronautasMortos();
            break;
        }
        case 0:
            std::cout << "Saindo\n";
            break;
        default:
            std::cout << "Essa opcao nao existe.\n";
        }
    } while (opcao != 0);

    return 0;
}

