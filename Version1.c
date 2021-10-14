//programa en C para consultar los datos de la base de datos
//Incluir esta libreria para poder hacer las llamadas en shiva2.upc.es
//#include <my_global.h>
#include <mysql.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
int main(int argc, char **argv)
{
	MYSQL *conn;
	int err;
	// Estructura especial para almacenar resultados de consultas 
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	int edad;
	char dni[10];
	char consulta [80];
	//Creamos una conexion al servidor MYSQL 
	conn = mysql_init(NULL);
	if (conn==NULL) {
		printf ("Error al crear la conexi??n: %u %s\n", 
				mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	//inicializar la conexion
	conn = mysql_real_connect (conn, "localhost","root", "mysql", "base_de_datos",0, NULL, 0);
	if (conn==NULL) {
		printf ("Error al inicializar la conexi??n: %u %s\n", 
				mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	// consulta SQL para obtener una tabla con todos los datos
	// de la base de datos
	err=mysql_query (conn, "SELECT * FROM jugadores");
	if (err!=0) {
		printf ("Error al consultar datos de la base %u %s\n", mysql_error(conn));
		exit (1);
	}
	//recogemos el resultado de la consulta. El resultado de la
	//consulta se devuelve en una variable del tipo puntero a
	//MYSQL_RES tal y como hemos declarado anteriormente.
	//Se trata de una tabla virtual en memoria que es la copia
	//de la tabla real en disco.
	resultado = mysql_store_result (conn);
	// El resultado es una estructura matricial en memoria
	// en la que cada fila contiene los datos de una persona.
	
	// Ahora obtenemos la primera fila que se almacena en una
	// variable de tipo MYSQL_ROW
	row = mysql_fetch_row (resultado);
	// En una fila hay tantas columnas como datos tiene una
	// persona. En nuestro caso hay tres columnas: dni(row[0]),
	// nombre(row[1]) y edad (row[2]).
	if (row == NULL)
		printf ("No se han obtenido datos en la consulta\n");
	else
		while (row !=NULL) {
			// la columna 2 contiene una palabra que es la edad
			// la convertimos a entero 
			// las columnas 0 y 1 contienen DNI y nombre 
			printf ("%s,%s,%s. \n", row[0], row[1], row[2]);
			// obtenemos la siguiente fila
			row = mysql_fetch_row (resultado);
	}
		
		
		printf ("Ganador:\n"); 
		strcpy (consulta,"SELECT Ganador FROM partidas WHERE id = 3");
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
			printf("%s. \n", row[0]);
		
		printf ("ID PALIZAS:\n"); 
		strcpy (consulta,"SELECT idPartida FROM relacion WHERE puntos1 > puntos2 + 30 OR puntos2 > puntos1 + 30");
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
			printf("ID: %s. \n", row[0]);
			row = mysql_fetch_row(resultado);
			}
			
			printf ("Líder:\n"); 
			strcpy (consulta,"SELECT Ganador FROM partidas GROUP BY Ganador ORDER BY COUNT(*) DESC LIMIT 1");
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
				printf("%s. \n", row[0]);
			
			
			printf ("Número de partidas empatadas:\n"); 
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
				printf("%s. \n", row[0]);
			
			
		mysql_close (conn);
		exit(0);
}
