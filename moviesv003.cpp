/*******************************************************************************************************
**              Aplicação C++ para obter informações sobre um determinado filme ou série de TV.
**                  
**--------------Informação do Arquivo -----------------------------------------------------------------
** Nome do Arquivo:          movies.cpp
** Data Ultima Modificação:  08-02-19
** Ultima Versão:            Sim
** Descrição:                Aplicação main para chamada de funções.   
**------------------------------------------------------------------------------------------------------
** Criado por:          Marlon Zanardi <marlon.zanardi95@hotmail.com>
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
#define ERRO 0
// Constante para retornos com sucesso.
#define SUCESSO 1

// Comando "search"
#define SEARCH "--search"
// Comando "help"
#define HELP "--help"
// Comando "type"
#define TYPE "--type"

// Tipo de busca de titulo.
#define FULL 1
#define TIPO 2

// API key
#define APIKEY "apikey=a89fa8e"

/* Prototipo. */
size_t curl_write(void *ptr, size_t size, size_t nmemb, void *stream);
void busca_titulo(string tmptitle, int tipo);

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
    }

    if ( !comand.compare(TYPE) ) 
    {
      // Serial de debug.
      #ifdef DEBUG
      cout << "Comando type" << endl;
      #endif
      // Confirma a quantidade de argumentos.
      if ( argc == 3 )
        // Pega o valor do argumento.
        title = argv[2];
      else
        cout << "Movie: parametro incorreto" << endl;

      // Chama função que faz a busca do titilo, com parametro TIPO(somente o tipo da media).
      busca_titulo(title,TIPO); 
    }
    if ( !comand.compare(SEARCH) ) 
    {
      // Serial de debug.
      #ifdef DEBUG
      cout << "Comando search" << endl;
      #endif 

      // Confirma a quantidade de argumentos.
      if ( argc == 3 )
        // Pega o valor do argumento.
        title = argv[2];
      else
        cout << "Movie: parametro incorreto" << endl;   

      // Chama função que faz a busca do titilo, com parametro FULL(completo).
      busca_titulo(title,FULL); 

     }	
  } 
  // Retorno da funcao main.
  return 0;
}

/*******************************************************************************************************
** Nome da Função:       busca_titulo
** Descrição: 		 Função que faz a chamada na API e retorna os valores desejados.
** Parametro:            tmptitle = Titulo a ser procurado,tipo = Tipo do retorno desejado.
** Valor de retorno:     Não.
*********************************************************************************************************/
void busca_titulo(string tmptitle, int tipo)
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
	cout << "Erro: Titulo não encontrado! Por favor cheque o nome ou tente outro titulo." << endl;
      else
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
	  // Mostra na tela os resultados.
	  cout << "Titulo: " << name.GetString() << " - " << year.GetString() << endl;	
	  cout << "Genero: " << genre.GetString() << endl;
	  cout << "Avaliacao: " << rating.GetString() << endl;
	  cout << "Diretor: " << director.GetString() << endl;
	  cout << "Atores: " << actors.GetString() << endl;
	  cout << "Duracao: " << runtime.GetString() << endl;
	  cout << "IMDB Link: " << "http://www.imdb.com/title/" << imdbId.GetString() << "/" << endl;
	  cout << "Sinopse: " << plot.GetString() << endl;	   	
	 }else if ( tipo == TIPO )
	 { 
	   Value& name = d["Title"];
           Value& type = d["Type"];
	   // Mostra na tela os resultados.
	   cout << "Titulo: " << name.GetString() << ". Tipo da midia: " << type.GetString() << endl;
	 }	
       }
     }
     // Mostra mensagem que não conseguiu retorno.
     catch(...)
     { 
       cout << "Não foi possivel obeter retorno, tente novamente!";
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

