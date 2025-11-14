#include <iostream>
#include <fstream> //leitura e escrita em arquivo
#include <string>
using namespace std;

enum Cor {VERMELHO, PRETO}; // enumeracao para as cores dos nós

class no { // nó da arvore
private:
    string palavra;
    string significado;
    no *esq, *dir, *pai; // ponteiros para pai e filhos
    Cor cor;

public:
    // construtor que inicializa atributos (palavra, significado, ponteiros comecando em null e cores)
    no(string p, string s) : palavra(p), significado(s), esq(nullptr), dir(nullptr), pai(nullptr), cor(VERMELHO) {}
    Cor getCor() {
        return cor;
    }
    void setCor(Cor c) {
        cor = c;
    }
    string getPalavra() {
        return palavra;
    }
    string getSignificado() {
        return significado;
    }
    void setSignificado(string s) {
        significado = s;
    }
    no* getEsq() {
        return esq;
    }
    no* getDir() {
        return dir;
    }
    no* getPai() {
        return pai;
    }
    void setPai(no* p) {
        pai = p;
    }
    void setEsq(no* n) {
        esq = n;
    }
    void setDir(no* n) {
        dir = n;
    }
};

class arvore { //arvore RB
private:
    no* raiz;

    void rotacaoEsquerda(no* node) {
        if (!node) return;
        no* y = node->getDir();
        if (!y) return;
        node->setDir(y->getEsq());
        if (node->getDir()) node->getDir()->setPai(node);
        y->setPai(node->getPai());
        if (!node->getPai()) raiz = y;
        else if (node == node->getPai()->getEsq()) node->getPai()->setEsq(y);
        else node->getPai()->setDir(y);
        y->setEsq(node);
        node->setPai(y);
    }

    void rotacaoDireita(no* node) {
        if (!node) return;
        no* x = node->getEsq();
        if (!x) return;
        node->setEsq(x->getDir());
        if (node->getEsq()) node->getEsq()->setPai(node);
        x->setPai(node->getPai());
        if (!node->getPai()) raiz = x;
        else if (node == node->getPai()->getDir()) node->getPai()->setDir(x);
        else node->getPai()->setEsq(x);
        x->setDir(node);
        node->setPai(x);
    }

    // verifica o balanceamento apos insercao
    void corrigirInsercao(no* node) {
        while (node != raiz && node->getPai()->getCor() == VERMELHO) {
            no* pai = node->getPai();
            no* avo = pai->getPai();
            if (pai == avo->getEsq()) { // pai é filho esquerdo do avo
                no* tio = avo->getDir();
                if (tio && tio->getCor() == VERMELHO) { // caso 1: tio vermelho
                    avo->setCor(VERMELHO);
                    pai->setCor(PRETO);
                    tio->setCor(PRETO);
                    node = avo;
                } else {
                    if (node == pai->getDir()) { // caso 2: nó é filho direito
                        rotacaoEsquerda(pai); 
                        node = pai;
                        pai = node->getPai();
                    }
                    rotacaoDireita(avo); // caso 3: nó é filho esquerdo
                    Cor tmp = pai->getCor();
                    pai->setCor(avo->getCor());
                    avo->setCor(tmp);
                    node = pai;
                }
            } else { // pai é filho direito do avo
                no* tio = avo->getEsq(); 
                if (tio && tio->getCor() == VERMELHO) { // caso 1: tio vermelho
                    avo->setCor(VERMELHO);
                    pai->setCor(PRETO);
                    tio->setCor(PRETO);
                    node = avo;
                } else {
                    if (node == pai->getEsq()) { // caso 2: nó e pai vermelhos
                        rotacaoDireita(pai);
                        node = pai;
                        pai = node->getPai();
                    }
                    rotacaoEsquerda(avo); // caso 3: nó é filho direito
                    Cor tmp = pai->getCor();
                    pai->setCor(avo->getCor());
                    avo->setCor(tmp);
                    node = pai;
                }
            }
        }
        raiz->setCor(PRETO); // raiz vermelha é pintada de preto
    }

        // verifica o balanceamento apos remocao
        void corrigirRemocao(no* node) {
        while (node != raiz && node->getCor() == PRETO) { 
            no* pai = node->getPai();
            if (node == pai->getEsq()) { // cassos onde o nó é filho esquerdo
                no* irmao = pai->getDir();
                if (irmao->getCor() == VERMELHO) { // caso 1: irmao vermelho
                    irmao->setCor(PRETO);
                    pai->setCor(VERMELHO);
                    rotacaoEsquerda(pai);
                    irmao = pai->getDir();
                }
                if ((irmao->getEsq() == nullptr || irmao->getEsq()->getCor() == PRETO) && // caso 2: ambos os filhos do 
                    (irmao->getDir() == nullptr || irmao->getDir()->getCor() == PRETO)) { // irmao sao pretos
                    irmao->setCor(VERMELHO);
                    node = pai;
                } else {
                    if (!irmao->getDir() || irmao->getDir()->getCor() == PRETO) { // caso 3: filho direito do irmao é preto
                        if (irmao->getEsq()) irmao->getEsq()->setCor(PRETO);
                        irmao->setCor(VERMELHO);
                        rotacaoDireita(irmao);
                        irmao = pai->getDir();
                    }
                    irmao->setCor(pai->getCor()); // caso 4: filho direito do irmao é vermelho
                    pai->setCor(PRETO);
                    if (irmao->getDir()) irmao->getDir()->setCor(PRETO);
                    rotacaoEsquerda(pai);
                    node = raiz;
                }
            } else { // casos onde o nó é filho direito
                no* irmao = pai->getEsq();
                if (irmao->getCor() == VERMELHO) { // caso 1: irmao vermelho
                    irmao->setCor(PRETO);
                    pai->setCor(VERMELHO);
                    rotacaoDireita(pai);
                    irmao = pai->getEsq();
                }
                if ((irmao->getEsq() == nullptr || irmao->getEsq()->getCor() == PRETO) && // caso 2: ambos os filhos do 
                    (irmao->getDir() == nullptr || irmao->getDir()->getCor() == PRETO)) { // irmao sao pretos
                    irmao->setCor(VERMELHO);
                    node = pai;
                } else {
                    if (!irmao->getEsq() || irmao->getEsq()->getCor() == PRETO) { // caso 3: filho esquerdo do irmao é preto
                        if (irmao->getDir()) irmao->getDir()->setCor(PRETO);
                        irmao->setCor(VERMELHO);
                        rotacaoEsquerda(irmao);
                        irmao = pai->getEsq();
                    }
                    irmao->setCor(pai->getCor()); // caso 4: filho esquerdo do irmao é vermelho
                    pai->setCor(PRETO);
                    if (irmao->getEsq()) irmao->getEsq()->setCor(PRETO);
                    rotacaoDireita(pai);
                    node = raiz;
                }
            }
        }
        if (node) node->setCor(PRETO);
    }

    // listar a arvore em ordem alfabetica
    void emOrdem(no* node) {
        if (node) {
            emOrdem(node->getEsq());
            cout << node->getPalavra() << " ";
            emOrdem(node->getDir());
        }
    }

    // encontra o nó com o menor valor
    no* encontraMin(no* node) { 
        while (node->getEsq()) node = node->getEsq();
        return node;
    }

    // substitui o nó u pelo nó v 
    void trocar(no* u, no* v) {  
        if (!u->getPai()) raiz = v;
        else if (u == u->getPai()->getEsq()) u->getPai()->setEsq(v);
        else u->getPai()->setDir(v);
        if (v) v->setPai(u->getPai());
    }

public:
    arvore() : raiz(nullptr) {}

    // insere uma nova palavra + significado
    void inserir(string p, string s) {
        no* z = new no(p, s);
        no* y = nullptr;
        no* x = raiz;

        while (x) {
            y = x;
            if (p < x->getPalavra()) x = x->getEsq();
            else if (p > x->getPalavra()) x = x->getDir();
            else {
                x->setSignificado(s); // atualiza se ja existe
                return;
            }
        }

        z->setPai(y);
        if (!y) raiz = z;
        else if (p < y->getPalavra()) y->setEsq(z);
        else y->setDir(z);

        corrigirInsercao(z);
    }
    
    void remover(string p) {
        no* z = raiz;
        while (z && z->getPalavra() != p) {
            if (p < z->getPalavra()) z = z->getEsq();
            else z = z->getDir();
        }
        if (!z) {
            cout << "Palavra não encontrada!\n";
            return;
        }

        no* y = z;
        no* x = nullptr;
        no* xp = nullptr;
        Cor corOriginal = y->getCor(); // guarda a cor original de y

        if (!z->getEsq()) { // caso 1: nó sem filho esquerdo
            x = z->getDir();
            xp = z->getPai();
            trocar(z, z->getDir());
        } else if (!z->getDir()) { // caso 2: nó sem filho direito
            x = z->getEsq();
            xp = z->getPai();
            trocar(z, z->getEsq());
        } else { // caso 3: nó com dois filhos
            y = encontraMin(z->getDir()); // encontra o sucessor de z
            corOriginal = y->getCor();
            x = y->getDir();
            if (y->getPai() != z) { 
                trocar(y, y->getDir());
                y->setDir(z->getDir());
                if (y->getDir()) y->getDir()->setPai(y);
            }
            trocar(z, y); // substitui z (sucessor) pelo nó removido
            y->setEsq(z->getEsq());
            if (y->getEsq()) y->getEsq()->setPai(y);
            y->setCor(z->getCor());
        }

        delete z; // deleta o nó removido
        if (corOriginal == PRETO) {
            if (x) corrigirRemocao(x);
            else if (xp) corrigirRemocao(xp);
        }
        if (raiz) raiz->setCor(PRETO);
    }

    string buscar(string p) {
        no* x = raiz;
        while (x) {
            if (p == x->getPalavra()) return x->getSignificado();
            if (p < x->getPalavra()) x = x->getEsq();
            else x = x->getDir();
        }
        return "";
    }

    void listar() { emOrdem(raiz); cout << endl; }

    void salvarArquivo(ofstream &out) {
        if (!raiz) return;
        salvarArvore(raiz, out);
    }

private: // salva arvore num arquivo
    void salvarArvore(no* node, ofstream &out) {
        if (!node) return;
        out << node->getPalavra() << " -> " << node->getSignificado() << endl;
        salvarArvore(node->getEsq(), out);
        salvarArvore(node->getDir(), out);
    }
};

int main(int argc, char* argv[]) {
    arvore dicionario;

    if (argc < 2) { // argumentos invalidos na entrada (deve ser ./programa arquivo-de-entrada)
        cout << "Uso: " << argv[0] << " <arquivo_entrada>\n";
        return 1;
    }

    ifstream infile(argv[1]); // abre o arquivo de entrada
    if (!infile) { // erro ao abrir o arquivo(nao existe, nome errado, etc)
        cout << "Não foi possível abrir o arquivo.\n"; 
        return 1; 
    }

    string palavra, significado;
    // le palavra e significado do arquivo (repete ate EOF)
    while (infile >> palavra >> ws && getline(infile, significado)) { // ws descarta espaços em branco antes do significado
        dicionario.inserir(palavra, significado);
    }
    infile.close(); // fecha arquivo

    int opcao; // menu de opcoes
    do {
        cout << "\n=== MENU ===\n";
        cout << "1 - Inserir nova palavra\n";
        cout << "2 - Buscar palavra\n";
        cout << "3 - Listar todas as palavras\n";
        cout << "4 - Excluir palavra\n";
        cout << "0 - Sair\n";
        cout << "Escolha: ";
        cin >> opcao;
        cin.ignore();

        switch(opcao) {
            case 1:
                cout << "Palavra: ";
                getline(cin, palavra);
                cout << "Significado: ";
                getline(cin, significado);
                dicionario.inserir(palavra, significado);
                break;
            case 2:
                cout << "Palavra: ";
                getline(cin, palavra);
                significado = dicionario.buscar(palavra);
                if (significado.empty()) cout << "Palavra não encontrada.\n";
                else cout << "Significado: " << significado << endl;
                break;
            case 3:
                cout << "Palavras em ordem: ";
                dicionario.listar();
                break;
            case 4:
                cout << "Palavra: ";
                getline(cin, palavra);
                dicionario.remover(palavra);
                break;
            case 0:
                break;
            default:
                cout << "Opção inválida.\n";
        }
    } while(opcao != 0);

    ofstream outfile("saida.txt"); // abre/cria arquivo de saida
    dicionario.salvarArquivo(outfile); // salva arvore no arquivo
    outfile.close(); 
    cout << "Árvore salva em saida.txt\n";
    return 0;
}