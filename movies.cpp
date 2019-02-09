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

/* Prototipo. */
size_t curl_write( void *ptr, size_t size, size_t nmemb, void *stream);

/*******************************************************************************************************
** Nome da Função:       main
** Descrição: 		 Função principal de execucao.
** Parametro:            argc = Quantidade de argumentos recebidos,*argc[] = Lista de argumentos recebidos.
** Valor de retorno:     1 = Falha, 0 = Final do programa.
*********************************************************************************************************/
int main(int argc, char *argv[])
{ 
  CURL *curl;
	CURLcode res;
	int start,i;
	string tmptitle,title,url;
	while(1){
		cout<<"\nEntre com o titulo que deseja pesquisar:";
		getline (std::cin,tmptitle); //take input
		if(tmptitle==""){ //if empty input, skip everything
			continue;
		}
		start = 0;
		//replace spaces with %2f
		for(i=0;i<tmptitle.length();i++){
			if(tmptitle[i] == ' '){
				title.append(tmptitle,start,i-start);
				title = title+"%2f";
				start = i+1;
			}
		}
		title.append(tmptitle,start,i-start);
		curl = curl_easy_init();
		url = "http://www.omdbapi.com/?t="+title+"&plot=full&apikey=a89fa8e"; //API URL
		cout<<"\nProcurando: "<<tmptitle<<"\n";
		cout<<"\nAguarde...\n\n";
		if(curl) {
			//set url to visit
			curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
			/* if URL is redirected, we tell libcurl to follow redirection */ 
			curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_write);
			/* Perform the request, res will get the return code */ 
			res = curl_easy_perform(curl);
			/* Check for errors */ 
			if(res != CURLE_OK){
				fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
			}
			try{
				//fwrite( buffer.c_str(), buffer.length(), sizeof(char), stdout); //uncomment to see raw JSON response
				if(buffer.length()<100){ //if response is too short, it's probably an error. DO NOT PARSE!
					cout<<"ERROR: Title Not Found! Please check the spelling or try another title\n";
				}
				else{
					// Parse the response and fill relevant variables
					const char* json = buffer.c_str();
					Document d;
					d.Parse(json);
					Value& name = d["Title"];
					Value& year = d["Year"];
					Value& genre = d["Genre"];
					Value& plot = d["Plot"];
					Value& runtime = d["Runtime"];
					Value& imdbId = d["imdbID"];
					Value& rating = d["imdbRating"];
					Value& director = d["Director"];
					Value& actors = d["Actors"];
					// Time to see the results
					cout<<"Title: "<<name.GetString()<<" - "<<year.GetString()<<endl;
					cout<<"Genre: "<<genre.GetString()<<endl;
					cout<<"Rating: "<<rating.GetString()<<endl;
					cout<<"Director: "<<director.GetString()<<endl;
					cout<<"Actors: "<<actors.GetString()<<endl;
					cout<<"Runtime: "<<runtime.GetString()<<endl;
					cout<<"IMDB Link: "<<"http://www.imdb.com/title/"<<imdbId.GetString()<<"/"<<endl;
					cout<<"Plot: "<<endl<<"-----------------------------------------------------------------------------\n";
					cout<<plot.GetString()<<endl;
				}
			}
			catch(...){ // catch before code dies
				//fwrite( buffer.c_str(), buffer.length(), sizeof(char), stdout); //uncomment to see raw JSON response
				cout<<"Bad luck!! Try Again!";
			}
			/* always cleanup */
			buffer.clear();
			curl_easy_cleanup(curl);
			/* empty the title for reuse in next iteration */
			title.erase(title.begin(),title.end());
		}
	}
	

  // Retorno da funcao main.
  return 0;
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

