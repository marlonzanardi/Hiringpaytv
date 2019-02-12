/*******************************************************************************************************
**              Aplicação C++ para obter informações sobre um determinado filme ou série de TV.
**                  
**--------------Informação do Arquivo -----------------------------------------------------------------
** Nome do Arquivo:          movies.cpp
** Data Ultima Modificação:  12-02-19
** Ultima Versão:            Sim
** Descrição:                Aplicação main para chamada da aplicação.
**------------------------------------------------------------------------------------------------------
** Criado por:          Marlon Zanardi <marlon.zanardi95@hotmail.com>
						https://www.linkedin.com/in/marlon-zanardi-965291147/
** Data de Criação:     08-02-19       
********************************************************************************************************/

/* Bibliotecas */
#include <iostream>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <stdlib.h>
#include <string>
#include <curl/curl.h>

/* Instancias */
using namespace std;
using namespace rapidjson;
std::string buffer;

/* Serial de debug. */
// Debug geral.
//#define DEBUG

/* Constantes */
// Constante para retornos de erro.
#define FALHA 1
// Constante para retornos com sucesso.
#define SUCESSO 0

// Comando "search"
#define SEARCH "--search"
// Comando "help"
#define HELP "--help"
// Comando "type"
#define TYPE "--type"

// Tipo de busca de titulo.
#define FULL 1
#define TIPO 2

// API key (Free key, somente 1k utilizaçoes diarias).
#define APIKEY "apikey=a89fa8e"

/* Prototipo. */
size_t curl_write(void *ptr, size_t size, size_t nmemb, void *stream);
bool busca_titulo(string tmptitle, int tipo);

/*******************************************************************************************************
** Nome da Função:       main
** Descrição: 		 Função principal de execucao.
** Parametro:            argc = Quantidade de argumentos recebidos,*argc[] = Lista de argumentos recebidos.
** Valor de retorno:     1 = Falha, 0 = Final do programa.
*********************************************************************************************************/
int main(int argc, char *argv[])
{ 
  string comand,title;

  // Verifca se existe a quantidade minia de parametros pra função.
  if ( argc < 2 )
    cout << "Movie: erro, utilize o comando --help para obter ajuda" << endl;
  else
  {
    // Joga o primeira parametro para comparação de comando.
    comand = argv[1];

    // Serial de debug.
    #ifdef DEBUG
    cout << "Comand: " << comand << endl;
    #endif

    // Verifica qual comando foi recebido.
    if ( !comand.compare(HELP) ) 
    {
      // Serial de debug.
      #ifdef DEBUG
      cout << "Comando help" << endl;
      #endif
      
      cout << "--help    Imprime a lista de opções da aplicação." << endl;
      cout << "--search  Busca pelo título da media." << endl;
      cout << "--type    Retorna o tipo media selecionado. (movie, series, episode)" << endl;     
      
      // Retorno da funcao main.
      return SUCESSO;
    }

    if ( !comand.compare(TYPE) ) 
    {
      // Serial de debug.
      #ifdef DEBUG
      cout << "Comando type" << endl;
      #endif
      // Confirma a quantidade de argumentos.
      if ( argc > 2 )
      {
	// Seta a variavel titulo em branco.
	title = "";
	// Laço para fazer a concatenação dos argumentos.
	for ( int i = 2; i < argc; i++)
	{
          // Pega o valor do argumento.
          title += argv[i];
	  // Concatena com espaço.
	  title += " ";
	}
	// Serial de debug.
        #ifdef DEBUG
        cout << "String titulo: " << title << endl;
        #endif
      }else
      {
        cout << "Movie: parametro incorreto" << endl;
        // Retorno da funcao main.
        return FALHA;
      }

      // Chama função que faz a busca do titilo, com parametro TIPO(somente o tipo da media).
      if ( busca_titulo(title,TIPO) ) 
      // Retorno da funcao main.
        return FALHA;
      else
	// Retorno para funcao main.
        return SUCESSO;
    }
    if ( !comand.compare(SEARCH) ) 
    {
      // Serial de debug.
      #ifdef DEBUG
      cout << "Comando search" << endl;
      #endif 

      // Confirma a quantidade de argumentos.
      if ( argc > 2 )
      {
	// Seta a variavel titulo em branco.
	title = "";
	// Laço para fazer a concatenação dos argumentos.
	for ( int i = 2; i < argc; i++)
	{
          // Pega o valor do argumento.
          title += argv[i];
	  // Concatena com espaço.
	  title += " ";
	}
	// Serial de debug.
        #ifdef DEBUG
        cout << "String titulo: " << title << endl;
        #endif
      }else
      {
        cout << "Movie: parametro incorreto" << endl;
        // Retorno da funcao main.
        return FALHA;
      }  

      // Chama função que faz a busca do titilo, com parametro FULL(completo).
      if ( busca_titulo(title,FULL) )        
        // Retorno da funcao main.
        return FALHA;
      else
	// Retorno para funcao main.
        return SUCESSO;
     }
     // Se não for nenhum comando existente.
     cout << "Movie: parametro incorreto" << endl; 
     // Retorno da funcao main.
     return FALHA;
  } 
}

/*******************************************************************************************************
** Nome da Função:       busca_titulo
** Descrição: 		 Função que faz a chamada na API e retorna os valores desejados.
** Parametro:            tmptitle = Titulo a ser procurado,tipo = Tipo do retorno desejado.
** Valor de retorno:     1 - Sucesso, 0 - Falha.
*********************************************************************************************************/
bool busca_titulo(string tmptitle, int tipo)
{  
  int start,i;
  CURL *curl;
  CURLcode res;
  string titlereplace,url;

  // Zera variavel de controle.    
  start = 0;
  // Substitui os espaços para poder enviar o comando em uma url.
  for( i=0; i<tmptitle.length(); i++)
  {
    // Procura o caractere ' ' na cadeia de caracteres da string.
    if(tmptitle[i] == ' ')
    {
      // Concatena com a nova substituição.
      titlereplace.append(tmptitle,start,i-start);
      titlereplace = titlereplace+"%2f";
      start = i+1;
    }
  }
  titlereplace.append(tmptitle,start,i-start);
  // Inicia a instancia do curl.
  curl = curl_easy_init();
  // Monta a URL de envio para API.
  url = "http://www.omdbapi.com/?t="+titlereplace+"&plot=full&"+APIKEY; 
  // Serial de debug.
  #ifdef DEBUG
  cout << "URL de envio: " << url << endl;
  #endif
  
  // Mostra mensagem para aguardar o retorno.
  cout << "Procurando titulo: " << tmptitle << endl;
  cout << "Aguarde..." << endl;

  // Se curl ativo.
  if( curl ) 
  {
    // Seta a url para visita.
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    // Se a URL é redirecionada, habilitamos para seguir o redirecionamento.  
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_write);
    // Retorna a res o código de resposta.
    res = curl_easy_perform(curl);
    // Chega se existe algum erro.
    if ( res != CURLE_OK )
    {
      fprintf(stderr, "curl_easy_perform() falhou: %s\n",curl_easy_strerror(res));
    }
    try
    {
      // Se a resposta for muito curta, provavelmente é um erro, então não continua.
      if ( buffer.length() < 100 )           
      {
	cout << "Erro: Titulo não encontrado! Por favor cheque o nome ou tente outro titulo." << endl;
        // Retorno para funcao main.
        return FALHA;
      }else
      {
	// Pega o retorno de alguns campos relevantes.
	const char* json = buffer.c_str();
	Document d;
	d.Parse(json);
	// Verifica de acordo com o tipo, o que deve ser mostrado.
        if (tipo == FULL)
	{	    
	  Value& name = d["Title"];
      Value& year = d["Year"];
	  Value& genre = d["Genre"];
	  Value& plot = d["Plot"];
	  Value& runtime = d["Runtime"];
	  Value& imdbId = d["imdbID"];
	  Value& rating = d["imdbRating"];
	  Value& director = d["Director"];
	  Value& actors = d["Actors"];
	  Value& rated = d["Rated"];
	  Value& released = d["Released"];
	  Value& writer = d["Writer"];
	  Value& language = d["Language"];
	  Value& country = d["Country"];
	  Value& awards = d["Awards"];
	  Value& boxoffice = d["BoxOffice"];
	  Value& production = d["Production"];
	  Value& website = d["Website"];
	  // Mostra na tela os resultados.
	  cout << "Titulo: " << name.GetString() << " - " << year.GetString() << endl;		   	
  	  cout << "Rated: " << rated.GetString() << endl;
  	  cout << "Lancamento: " << released.GetString() << endl;
	  cout << "Duracao: " << runtime.GetString() << endl;
	  cout << "Genero: " << genre.GetString() << endl;
	  cout << "Diretor: " << director.GetString() << endl;
	  cout << "Escritor: " << writer.GetString() << endl;
	  cout << "Atores: " << actors.GetString() << endl;
	  cout << "Sinopse: " << plot.GetString() << endl;
	  cout << "Lingua: " << language.GetString() << endl;
	  cout << "Pais: " << country.GetString() << endl;
	  cout << "Premios: " << awards.GetString() << endl;
	  cout << "Avaliacao: " << rating.GetString() << endl;
	  cout << "Bilheteria: " << boxoffice.GetString() << endl;
	  cout << "Producao: " << production.GetString() << endl;
	  cout << "Website: " << website.GetString() << endl;
	  cout << "IMDB Link: " << "http://www.imdb.com/title/" << imdbId.GetString() << "/" << endl;
	 }else if ( tipo == TIPO )
	 { 
	   Value& name = d["Title"];
           Value& type = d["Type"];
	   // Mostra na tela os resultados.
	   cout << "Titulo: " << name.GetString() << ". Tipo da midia: " << type.GetString() << endl;
	 }
	 // Retorno para funcao main.
         return SUCESSO;	
       }
     }
     // Mostra mensagem que não conseguiu retorno.
     catch(...)
     { 
       cout << "Não foi possivel obeter retorno, tente novamente!";
       // Retorno para funcao main.
       return FALHA;
     }
  }
}


/*******************************************************************************************************
** Nome da Função:       curl_write
** Descrição: 		 Função que pega os componentes de envio do comando curl e retorna o parametro
			 size para configuração do serviço.
** Valor de retorno:     (size.nmemb)
*********************************************************************************************************/
size_t curl_write( void *ptr, size_t size, size_t nmemb, void *stream)
{
  buffer.append((char*)ptr, size*nmemb);
  // Retorno da função.
  return size*nmemb;
}

