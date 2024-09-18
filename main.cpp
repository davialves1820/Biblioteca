#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>


using std::cout;
using std::endl;
using std::string;
using std::vector;

class Pessoa {
    string nome;

public:
    Pessoa(const string& n) : nome(n) {}

    string get_nome() { return nome; }
};

class Item {
protected:
    string nome;

public:
    Item(const string& n) : nome(n) {}

    string get_nome() const { return nome; }
};

class Livro : public Item {
    Pessoa autor; 
    bool disponivel;
public:
    Livro(const string& nome, const Pessoa& a) : Item(nome), autor(a), disponivel(true) {}

    bool get_disponivel() const { return disponivel; }

    void set_disponivel() { 
        if (disponivel) {
            disponivel = false;
        } else {
            disponivel = true;
        }
    }

    void print_info() {
        cout << "**********" << endl;
        cout << "Livro: " << nome << endl;
        cout << "Autor: " << autor.get_nome() << endl;
        string situacao;
        if (disponivel) { 
            situacao = "Disponivel para emprestimo"; 
        } else {
            situacao = "Indisponivel para emprestimo";
        }
        cout << "Situacao: " << situacao << endl;
    }
};

class Biblioteca {
    string nome;
    vector<std::shared_ptr<Livro>> livros;
public:
    Biblioteca(const string& n) : nome(n) {}

    void adicionar_livro(std::shared_ptr<Livro> livro) {
        livros.push_back(livro);
    }

    void remover_livro(const std::string& titulo) {
    livros.erase(std::remove_if(livros.begin(), livros.end(), //std::remove_if utiliza iteradores para percorrer o container. livros.begin() e livros.end() definem o intervalo de elementos a serem percorridos.
                                
                                [titulo](const std::shared_ptr<Livro>& livro) { //O terceiro argumento de std::remove_if é um predicado, ou seja, uma função que retorna um valor booleano. Neste caso, o predicado é um lambda que verifica se o nome do livro atual é igual ao título passado como parâmetro.
                                    
                                    if (livro->get_nome() == titulo && !livro->get_disponivel()) { // garante que se o livro estiver em um emprestimo ativo nao seja removido
                                        cout << "Livro esta em emprestimo nao pode ser removido" << endl;
                                        return false;
                                    } else if(livro->get_nome() == titulo) {
                                        cout << "Livro removido com sucesso." << endl;
                                        return true;
                                    } else {
                                        cout << "Livro nao encontrado" << endl;
                                        return false;
                                    }
                                    
                                }),
                livros.end()); //O resultado de std::remove_if é usado para ajustar o tamanho do container livros. O método erase é chamado com os iteradores retornados por std::remove_if e livros.end(), removendo os elementos que foram "movidos" para o final do container.
    }

    std::shared_ptr<Livro> get_livro(const string& livro) {
        for (auto& l: livros) { //percorre o vector de livros em busca de um livro com o nome referenciado
            if (l->get_nome() == livro) {
                return l;
            }
        }
        return nullptr;
    }

    void listar_livros() const {
        cout << "\tLivros" << endl;
        for (auto& livro: livros) {
            livro->print_info();
        }
    }
};

class Emprestimo {
    Pessoa* usuario;
    std::shared_ptr<Livro> livro;
public:
    Emprestimo(Pessoa* u) : usuario(u), livro(nullptr){}


    void realizar_emprestimo(std::shared_ptr<Livro> l) {

        if (l->get_disponivel()) {
            livro = l;
            livro->set_disponivel();
            cout << "Emprestimo realizado" << endl;
        } else {
            cout << "Livro indisponivel para emprestimo." << endl;
        }
    }

    void devolver_livro() {
        livro->set_disponivel();
        cout << "Livro devolvido com sucesso" << endl; 
        livro = nullptr;
    }

};

void menu_opcao() {
    cout << "---------------------------" << endl;
    cout << "\tMENU OPCOES" << endl;
    cout << "1-Cadastrar livro" << endl;
    cout << "2-Remover livro" << endl;
    cout << "3-Listar livros" << endl;
    cout << "4-Realizar emprestimo" << endl;
    cout << "5-Devolver livro" << endl;
    cout << "0-Sair" << endl;
    cout << "Opcao: ";
}


int main() {

    int opcao; //variavel para guardar a opcao do usuario
    bool emprestimo_ativo = false; //varial auxiliar para verificar se o usuario ja tem um emprestimo em execucao
    Biblioteca biblioteca("Biblioteca do centro de informatica");
    Pessoa davi("Davi"); //usuario
    Emprestimo emprestimo(&davi); //variavel para controlar os emprestimos do usuario
    do {
        
        menu_opcao();
        std::cin >> opcao;
        std::cin.ignore();
        cout << endl;

        switch(opcao) {
            case 1: { //adiciona livro a biblioteca
                cout << "Diga o nome do livro:";
                string nome;
                std::getline(std::cin, nome);
                cout << "Diga o nome do autor: ";
                string autor;
                std::getline(std::cin, autor);
                biblioteca.adicionar_livro(std::make_shared<Livro>(nome, autor));
                break;
            }
            case 2: { //remove livro da biblioteca
                cout << "Diga o nome do livro que deseja remover: ";
                string nome;
                std::getline(std::cin, nome);
                biblioteca.remover_livro(nome);
                break;
            }
            case 3: { //exibe todos os livros da biblioteca
                biblioteca.listar_livros();
                break;
            }
            case 4: { //realiza o emprestimo de um livro
                if (!emprestimo_ativo) {
                    cout << "Diga o nome do livro que deseja pegar para emprestimo: ";
                    string nome;
                    std::getline(std::cin, nome);
                    std::shared_ptr<Livro> livro = biblioteca.get_livro(nome);
                    if (livro == nullptr) {
                        cout << "Livro nao encontrado" << endl;
                    } else {
                        emprestimo.realizar_emprestimo(livro);
                        emprestimo_ativo = true;
                    }
                    
                } else {
                    cout << "E preciso devolver o livro atual para realizar um novo emprestimo" << endl;
                }
                
                break;
            }
            case 5: { //devolve o livro pego para emprestimo
                emprestimo.devolver_livro();
                emprestimo_ativo = false;
                break;
            }
        }
        

    } while(opcao != 0);

    return 0;

}