## Processo seletivo para vaga desenvolvedor C++

### Requisitos

* C/C++ em ambiente GNU/Linux

* Versionamento de código com Git

* Redes TCP/IP

* Linguagens de script (python, shell, perl)

* Conhecimento em ferramentas de Debug e otimização

### Desejáveis

* Conhecimento em TV digital, IPTV e DVB

* Biblioteca QT

* Programação em ambiente web (HTML, CSS, JavaScript)

### O Teste

Construa uma aplicação interativa, na qual o usuário possa através da linha de comando, obter
informações sobre um determinado filme ou série de TV.
As seguintes opções são esperadas:
* - h - - help
 Imprime a lista de opções da aplicação.
* -s - - search
 Busca pelo título da media.
* -t - - type
 Retorna o tipo media selecionado. (movie, series, episode)
 
### Exemplo de saída:

* ./movies - -search Guardians of the Galaxy

Title - Guardians of the Galaxy Vol. 2

Year - 2017

Rated - PG-13

Released - 05 May 2017

Runtime - 136 min

Genre - Action, Adventure, Comedy, Sci-Fi

Director - James Gunn

Writer - James Gunn, Dan Abnett (based on the Marvel comics by), Andy Lanning (based on the Marvel comics by), Steve
Englehart (Star-Lord created by), Steve Gan (Star-Lord created by), Jim Starlin (Gamora and Drax created by), Stan Lee (Groot
created by), Larry Lieber (Groot created by), Jack Kirby (Groot created by), Bill Mantlo (Rocket Raccoon created by), Keith Giffen
(Rocket Raccoon created by), Steve Gerber (Howard the Duck created by), Val Mayerik (Howard the Duck created by)
Actors - Chris Pratt, Zoe Saldana, Dave Bautista, Vin Diesel
Plot - The Guardians struggle to keep together as a team while dealing with their personal family issues, notably Star-Lord's
encounter with his father the ambitious celestial being Ego.

Language - English

Country - USA

Awards - Nominated for 1 Oscar. Another 12 wins & 42 nominations.

Poster - https://m.media-amazon.com/images/M/MV5BMTg2MzI1MTg3OF5BMl5BanBnXkFtZTgwNTU3NDA2MTI@._V1_SX300.jpg

Ratings -

Internet Movie Database : 7.7/10

Rotten Tomatoes: 83%

Metacritic: 67/100

Type - movie

DVD - 22 Aug 2017

BoxOffice -$389,804,217

Production - Walt Disney Pictures

Website - https://marvel.com/guardians

### Instruções:

* Os dados estão disponíveis através da API RESTful OMDb: http://www.omdbapi.com/

* A aplicação deve ser desenvolvida em C++.

* Valorizamos muito os testes unitários. Leve isso em consideração.

* Nos envie o código através de um repositório Git. (GitHub, Bitbucket, etc)

### Instruções de utilização:

* Para compilar o arquivo .cpp utilizar a seguinte linha de comando: g++ movies.cpp -o movies -lcurl

* É necessario a pasta rapidjson estar no repósitório para utilização da biblioteca

* É recomendado ter os pacotes de serviço curl atualizado em seu OS.

* Para utlitilar a aplicação, você deve enviar os parametros da aplicação como argumentos do executavel, por exemplo, ./movies --search Guardians of the Galaxy.

### Comandos disponiveis: 

* --help(mostra comandos disponiveis) não ha necessidade de enviar arumentos.

* --search(faz a busca do titulo desejado e mostra informações relevantes) deve ser enviado o nome do titulo como proximo argumento.

* --type(faz a busca do titulo desejado, mostrando o tipo do filme, se é um filme, serie ou epsodio) deve ser enviado o nome do titulo como proximo argumento.