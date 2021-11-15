#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <mysql.h>
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
typedef struct{
	char nombre[20];
	int id;
}Jugador;

typedef struct{
	Jugador jugadores[100];
	int num;
}ListaJugadores;


MYSQL *conn;
int i;
int sockets[100];
int cambioConectados = 0;

int PonConectado (ListaJugadores *lista, char nombre [20], int id)
{
	if (lista->num<100)
	{strcpy (lista->jugadores[lista->num].nombre,nombre);
	lista->jugadores[lista->num].id=id;
	lista->num=lista->num+1;
	cambioConectados = 1;
	return 0;
	}
	else 
		 return -1;
}
int EliminaConectado(ListaJugadores *lista, char nombre[20])
{ int cont=0;int n=0;
	while ((cont<lista->num) && (!n)){
		if (strcmp(lista->jugadores[cont].nombre,nombre)==0)
			n=1;
		else
			cont++;
	}
	if (n=0)
		return -1;
	else
	{
		for (int i = cont; i < lista->num-1; i++)
		{
			lista->jugadores[i]= lista->jugadores[i+1];
		}
		lista->num =lista->num-1;
		cambioConectados = 1;
		return 0;
	}
}
void conectar_base_de_datos(){
	// Estructura especial para almacenar resultados de consultas 
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	//Creamos una conexion al servidor MYSQL 
	conn = mysql_init(NULL);
	if (conn==NULL) {
		printf ("Error al crear la conexi??n: %u %s\n", 
				mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	//inicializar la conexion
	conn = mysql_real_connect (conn, "localhost","root", "mysql", "bd",0, NULL, 0);
	if (conn==NULL) {
		printf ("Error al inicializar la conexi??n: %u %s\n", 
				mysql_errno(conn), mysql_error(conn));
		exit (1);
		
	}
}

void Login_Usuario(char resp[512],char nombre[60], char contrasenya[20], ListaJugadores *lista,int id){
	{//registrar un usuaario 
	// Estructura especial para almacenar resultados de consultas 
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	char consulta[500];
	int err;
	
	sprintf(consulta, "SELECT jugadores.Id FROM jugadores WHERE jugadores.nombre = '%s' AND jugadores.contrasenya = '%s'", nombre, contrasenya);
	err = mysql_query(conn, consulta);
	if (err!=0) {
		printf ("Error al consultar datos de la base %u %s\n",
				mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	resultado = mysql_store_result(conn);
	row = mysql_fetch_row (resultado);
	if (row==NULL){
		printf("ERROR");
	}
	else {
		id=atoi(row[0]);
		strcpy(resp, row[0]);
		
		//int f = PonConectado(t, nombre, row[0]);
	}
}
}
void Registrar(char respuesta[512], char nombre[60], char contrasenya[20]){
	printf("1\n");
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	char consulta[300];
	char consulta2[300];
	int id;
	int err;
	sprintf(consulta, "SELECT MAX(id) FROM jugadores");
	err=mysql_query (conn, consulta); 
	if (err!=0) {
		printf ("Error al consultar datos de la base %u %s\n",
				mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	printf("2\n");
	resultado = mysql_store_result (conn);
	row = mysql_fetch_row (resultado);
	printf("3\n");
	id = atoi(row[0]) + 1;
	sprintf(consulta2, "INSERT INTO jugadores (id, nombre, contrasenya) VALUES (%d,'%s','%s')", id, nombre, contrasenya);
	printf("4\n");
	err=mysql_query (conn, consulta2); 
	printf("5\n");
	
	if (err!=0) {
		printf ("Error al consultar datos de la base %u %s\n",
				mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	resultado = mysql_store_result (conn);
	row = mysql_fetch_row (resultado);
	if (row==NULL)
		printf("ERROR");
	else
	{
		printf("6\n");
		strcpy (respuesta, "Registrado.");
	}
}
int GanadorPartidaspormas30puntos()
{
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	char consulta[100];
	int err;
	int cont;
	//Creamos una conexion al servidor MYSQL
	strcpy (consulta,"SELECT Idpartidas FROM relacion WHERE Puntos1 >= Puntos2 + 30 OR Puntos2 >= Puntos1 + 30");
	err=mysql_query (conn, consulta); 
	resultado = mysql_store_result (conn);
	row = mysql_fetch_row (resultado);
	if (err!=0) {
		printf ("Error al consultar datos de la base %u %s\n",
				mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	else
		while(row!=NULL){
		cont++;
		row = mysql_fetch_row(resultado);
		
	}
		return cont;
}
void LiderActual(char respuesta[512])		
{
	// Estructura especial para almacenar resultados de consultas 
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	char consulta[80];
	int err;
	//Creamos una conexion al servidor MYSQL 
	conectar_base_de_datos(&conn);
	strcpy (consulta,"SELECT COUNT(*) FROM partidas WHERE ganador=''");
	err=mysql_query (conn, consulta); 
	if (err!=0) {
		printf ("Error al consultar datos de la base %u %s\n",
				mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	resultado = mysql_store_result (conn);
	row = mysql_fetch_row (resultado);
	if (row==NULL)
		printf("ERROR");
	else
		strcpy(respuesta, row[0]);
}
void PartidasEmpatads (char respuesta[512]){
	// Estructura especial para almacenar resultados de consultas 
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	char consulta[80];
	int err;
	sprintf(consulta, "SELECT Ganador FROM partidas GROUP BY Ganador ORDER BY COUNT(*) DESC LIMIT 1");
	err=mysql_query (conn, consulta); 
	if (err!=0) {
		printf ("Error al consultar datos de la base %u %s\n",
				mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	resultado = mysql_store_result (conn);
	row = mysql_fetch_row (resultado);
	if (row==NULL) 
		printf("ERROR");
	
	else 
		strcpy(respuesta, row[0]);
}

void AtenderCliente(void *socket)
{
	int sock_conn = *(int*) socket;
	ListaJugadores lista; 
	lista.num=0;
	char peticion[512];
	char respuesta[512];
	int err;
	int f, i, ret;
	conectar_base_de_datos();
	char consulta[500];
	// Entramos en un bucle para atender todas las peticiones de este cliente
	//hasta que se desconecte
	int terminar =0;
	while (terminar ==0)
	{
		// Ahora recibimos la petici?n
		
		ret=read(sock_conn,peticion, sizeof(peticion));
		printf ("Recibido\n");
		
		// Tenemos que a?adirle la marca de fin de string 
		// para que no escriba lo que hay despues en el buffer
		peticion[ret]='\0';
		
		
		printf ("Peticion: %s\n",peticion);
		char *p = strtok( peticion, "/");
		int codigo =  atoi (p);
		// Ya tenemos el c?digo de la petici?n
		char nombre[60];
		char contrasenya[20];
		int id;
		if ((codigo == 1) || (codigo == 2))
		{
			
			p = strtok( NULL, "/");
			strcpy (nombre, p);
			p = strtok(NULL, "/*/");
			strcpy (contrasenya, p);
			printf ("Codigo: %d, Nombre: %s, Contrasenya: %s\n", codigo, nombre, contrasenya);
		}
		
		if (codigo == 0){
			terminar = 1;
			pthread_mutex_lock(&mutex);
			EliminaConectado(&lista,nombre);
			pthread_mutex_unlock(&mutex);
			
		}
		if (codigo == 1)
		{	pthread_mutex_lock(&mutex);
		Login_Usuario(respuesta, nombre, contrasenya, &lista,id);		
		PonConectado (&lista,nombre,id);
		pthread_mutex_unlock(&mutex);
		}
		if (codigo == 2)
		{ 	pthread_mutex_lock(&mutex);
		Registrar(respuesta, nombre,contrasenya);
		pthread_mutex_unlock(&mutex);
		}
		if (codigo ==3)
		{
			pthread_mutex_lock(&mutex);
			PartidasEmpatads(respuesta);
			pthread_mutex_unlock(&mutex);
		}
		if (codigo ==5)
		{
			pthread_mutex_lock(&mutex);
			int d = GanadorPartidaspormas30puntos();
			sprintf(respuesta, '%d', d);
			pthread_mutex_unlock(&mutex);
		}
		if (codigo ==4)
		{
			pthread_mutex_lock(&mutex);
			LiderActual(respuesta);
			pthread_mutex_unlock(&mutex);
		}
		
		if (codigo !=0)
		{
			pthread_mutex_lock(&mutex);
			printf ("Respuesta: %s\n", respuesta);
			// Enviamos respuesta
			write (sock_conn, respuesta, strlen(respuesta));
			pthread_mutex_unlock(&mutex);
		}
	}
	// Se acabo el servicio para este cliente
	close(sock_conn); 
	
	
}

int main(int argc, char *argv[])
{	
	int sock_conn, sock_listen;
	struct sockaddr_in serv_adr;
	
	if ((sock_listen = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		printf("Error creant socket");
	// Fem el bind al port
	
	
	memset(&serv_adr, 0, sizeof(serv_adr));// inicialitza a zero serv_addr
	serv_adr.sin_family = AF_INET;
	
	// asocia el socket a cualquiera de las IP de la m?quina. 
	//htonl formatea el numero que recibe al formato necesario
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	// establecemos el puerto de escucha
	serv_adr.sin_port = htons(9018);
	if (bind(sock_listen, (struct sockaddr *) &serv_adr, sizeof(serv_adr)) < 0)
		printf ("Error al bind");
	
	if (listen(sock_listen, 3) < 0)
		printf("Error en el Listen");
	
	pthread_t thread[100];
	// Bucle infinito
	for (;;){
		printf ("Escuchando\n");
		
		sock_conn = accept(sock_listen, NULL, NULL);
		printf ("He recibido conexion\n");
		//sock_conn es el socket que usaremos para este cliente
		sockets[i] = sock_conn;
		pthread_create (&thread[i], NULL, AtenderCliente, &sockets[i]);
		i = i + 1;
	}
}
