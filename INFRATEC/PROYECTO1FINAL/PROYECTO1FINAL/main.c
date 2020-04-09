// ISIS-1304 - FUNDAMENTOS DE INFRAESTRUCTURA TECNOLOGICA - PROYECTO 1 - 201910
// EL GRUPO DEBE SER DESARROLLADO EN GRUPOS DE A 3 PERSONAS MAXIMO
//
// DESARROLLADO POR:
// Andres Felipe Orozco Gonzalez - 201730058 (af.orozcog@uniandes.edu.co)
// Juan Andres Avelino Ospina - 201812676  (ja.avelino@uniandes.edu.co)
// Nicolas Andres Tobo Urrutia - 201817465  (na.tobo@uniandes.edu.co)

#define _CRT_SECURE_NO_DEPRECATE 
#include <stdlib.h> 
#include <stdio.h>
#include <string.h>

// La representacion de la imagen
typedef struct img
{
	int ancho;
	int alto;
	unsigned char *informacion;
} Imagen;


// Funcion que carga el bmp en la estructura Imagen
void cargarBMP24(Imagen * imagen, char * nomArchivoEntrada);

// Funcion que guarda el contenido de la estructura imagen en un archivo binario
void guardarBMP24(Imagen * imagen, char * nomArchivoSalida);

//Funcion que inserta un mensaje en la imagen usando n bits por Byte
void insertarMensaje(Imagen * img, unsigned char mensaje[], int n);

//Funcion que lee un mensaje de una imagen dando la longitud del mensaje y el numero de bits por byte usados
void leerMensaje(Imagen * img, unsigned char msg[], int l, int n);

//Funcion que saca n bits de una secuencia de caracteres a partir de una posici�n dada
unsigned char sacarNbits(unsigned char secuencia[], int bitpos, int n);

// Programa principal
// NO MODIFICAR
int main(int argc, char* argv[]) {
	//Malloc es una funcion que reserva un espacio en memoria para una variable tipo apuntador. 
	Imagen *img = (Imagen *)malloc(sizeof(Imagen)); //Arreglo de caracteres para almacenar el mensaje
	char msg[10000];
	int op, num, l, i, n;
	char nomArch[256];//Arreglo de caracteres para almacenar el nombre del archivo

	printf("ISIS-1304 - FUNDAMENTOS DE INFRAESTRUCTURA TECNOLOGICA - PROYECTO 1\n");
	printf("Indique la accion a realizar:\n\t1) Insertar mensaje\n\t2) Leer mensaje\n\n");
	printf("Opcion: ");
	scanf("%d", &op);//guarde en la posicion de memoria de op el valor dado por consola

	printf("Ingrese el nombre/ruta de la imagen (incluya el formato .bmp): ");

	char temp;
	scanf("%c", &temp);
	gets(nomArch); // Es similar a scanf ("%s",nomArch), pero permite espacios intermedios

	// Cargar los datos
	cargarBMP24(img, nomArch);

	if (op == 1) {
		printf("Ingrese el mensaje a insertar:\n\n");

		gets(msg);

		printf("\nLongitud mensaje: %d bytes\n", strlen(msg));
		msg[strlen(msg)] = '\0';

		printf("Ingrese el numero de bits por Byte: ");
		scanf("%d", &num);

		printf("Insertando . . .\n");
		insertarMensaje(img, msg, num);
		printf("Insertado correctamente !\n");

		char nuevo[256];
		printf("Ingrese el nombre del archivo a guardar (incluya el formato .bmp): ");

		scanf("%c", &temp);
		gets(nuevo);

		guardarBMP24(img, nuevo);

		printf("Mensaje insertado y guardado exitosamente en el archivo/ruta '%s' ! :D\n\n", nuevo);
	}
	else if (op == 2) {
		printf("Ingrese la longitud a leer del mensaje insertado: ");

		scanf("%d", &l);

		printf("Ingrese el numero de bits por Byte: ");
		scanf("%d", &n);

		for (i = 0; i < l; i++) {
			msg[i] = 0;
		}

		leerMensaje(img, msg, l, n);

		msg[l] = 0;

		printf("Mensaje obtenido exitosamente ! El mensaje es:\n\n%s\n\n", msg);
	}
	else {
		printf("La opcion ingresada (%d) no es correcta\n", op);
		system("pause");
		return -1;
	}
	system("pause");
	return 0;
}
/**
* Muestra el numero binario de un unsigned char
*/
void showBinary(unsigned char a) {
	while (a) {
		if (a & 1)
			printf("1");
		else
			printf("0");
		a = a >> 1;
	}
	printf("\n");
}


/**
* Inserta un mensaje, de a n bits por componente de color, en la imagen apuntada por img
* par�metro img: Apuntador a una imagen en cuyos pixeles se almacenar� el mensaje.
* par�metro mensaje: Apuntador a una cadena de caracteres con el mensaje.
* par�metro n: Cantidad de bits del mensaje que se almacenar�n en cada componente de color de cada pixel. 0 < n <= 8.
*/
void insertarMensaje(Imagen * img, unsigned char mensaje[], int n)
{
	// TODO: Desarrollar OBLIGATORIAMENTE en su totalidad.
	int width = (*img).ancho;
	int length = (*img).alto;
	unsigned char *image = (*img).informacion;
	int counter = 0; //Cantidad de bits insertados
	int index = 0; //Char en el que va
	int flag = 0; //bandera para indicar Caso final.
	//Recorre la estructura de la imagen en ancho y en alto
	for (int i = 0; i < length; i++) {
		for (int j = 0; j < width; j++) {
			unsigned char bitschar;     //Variable donde se guardan los bits de la imagen.
			unsigned char helper = 255; //Constante de ayuda para operaciones(Tamano del byte-1).
			unsigned char lector; //Lector de chars
			if (mensaje[index] == '\0') { //Si estoy en el caracter final
				flag = 1;
				break;
			}
			if (mensaje[index + 1] == '\0' && (((index + 1) * 8) - counter) < n) { //Si el que sigue es el caracter final y el numero de bits en counter es menor a la cantidad de bits del mensaje a insertar.
				bitschar = sacarNbits(mensaje, counter, n); //Saca los bits de un char
				bitschar = bitschar << (n - (((index + 1) * 8) - counter)); //Corrimiento izquierda para acomodar el bitchar a la posicion a la izq correcta.
				helper = helper << n; //Realiza un corrimiento de n bits sobre la variable helper (poner en 0 los ultimos n bits)
				*image = (*image) & helper;// Blanquea los ultimos n bits de la imagen para insertar el bitChar
				lector = *image; 
				*image = lector | bitschar; //inserta en la imagen los bits del char a codificar
				lector = *image;
				showBinary(*image);
				flag = 1; //
				break;
			}
			bitschar = sacarNbits(mensaje, counter, n); //Le devuelve un char con los bits a insertar al final 
			helper = helper << n; //Realiza un corrimiento de n bits sobre la variable helper
			*image = (*image) & helper; //Coloca ceros en las ultimas n posiciones del color
			lector = *image; 
			*image = lector | bitschar; //Le inserta los n bits a la imagen en el color
			lector = *image;
			image++; //Cambia al siguiente color
			counter += n; // Le suma al counter cuantos bits se han tomado
			index = counter / 8; // Me da la posicion del char en el que voy
		}
		if (flag)  //Recordar que si flag!=0 equivale a un true
			break;
	}
}

/**
* Extrae un mensaje de tamanio l, guardado de a n bits por componente de color, de la imagen apuntada por img
* par�metro img: Apuntador a una imagen que tiene almacenado el mensaje en sus pixeles.
* par�metro msg: Apuntador a una cadena de caracteres donde se depositar� el mensaje.
* par�metro l: Tamanio en bytes del mensaje almacenado en la imagen.
* par�metro n: Cantidad de bits del mensaje que se almacenan en cada componente de color de cada pixel. 0 < n <= 8.
*/
void leerMensaje(Imagen * img, unsigned char msg[], int l, int n) {
	// TODO: Desarrollar OBLIGATORIAMENTE en su totalidad.
	unsigned char *image = img->informacion; //Apuntador que obtiene la direccion de informacion de la estrutura img
	int toTakeLeft = l * 8; //Cuenta de bits que faltan por leer (GLOBAL)
 	int index = 1; //index+1, para saber que parametro de posicion utilizar para sacar N bits
	int left = 8; //Cuantos me faltan por tomar para tomar de un char completo
	unsigned char toAdd = 0; //Char vacio para rellenar con los bits del mensaje codificado. 
	int flag = 0; //Caso especial en que no se pueden recuperar n cantidad de bits
	int space = n; //En cuantos de los ultimos bits quedo el mensaje
	int lastTaken = 0;  //Siempre toma los ultimos bits que ingreso al mensaje
	while (toTakeLeft > 0) {
		if (flag) {
			int toTake = space - lastTaken; //Lasttaken cantidad de bits que se tomo por ultima vez.
			unsigned char valor = sacarNbits(image, (8 * index) - toTake, toTake);
			toAdd = toAdd | valor;
			lastTaken = toTake; //Bits que se usaron anteriormente
			left -= toTake; //Variable que remueve los bits usados recientemente
			toTakeLeft -= toTake;
			index++;
			flag = 0;
			if (left < space) {
				toAdd = toAdd << left;
			}
			else
				toAdd = toAdd << space;
		}
		else if (toTakeLeft < space) {
			unsigned char valor = sacarNbits(image, (8 * index) - space, toTakeLeft);
			toAdd = toAdd | valor;
			*msg = toAdd;
			msg++;
			toTakeLeft = 0;
		}
		else if (left >= space) { 
			unsigned char valor = sacarNbits(image, (8 * index) - space, space);
			toAdd = toAdd | valor;
			lastTaken = space;//Siempre toma los ultimos bits que tome de la variable space.
			left -= space; //Los bits que me faltan para completar el char
			toTakeLeft -= space; 
			if (left == 0) { //Ya quedo completo el mensaje
				*msg = toAdd; //Le añade al mensaje la posicion actual el char toAdd
				msg++;    //Mueve el apuntador a la siguiente posicion
				toAdd = 0; //Inicializa el 0 para la siguiente letra.
				left = 8; //Inicializa en 8 porque tiene que leer los 8 bits del otro mensaje.
			}
			else if (left < space) {
				toAdd = toAdd << left; //Mover lo que le falta a left para que no se borren los bits
			}
			else
				toAdd = toAdd << space; //Corre el toAdd anterior para que no se sobreEscriba.
			index++; //Actualiza el valor global para sacar la posicion
		}
		else if (left < space) 
		{
			lastTaken = left; //Toma los ultimos bits que tome 
			unsigned char valor = sacarNbits(image, (8 * index) - space, left); //Bits que se toman dentro del rango de (8 * index) - space y left .
			toAdd = toAdd | valor; //Une todos los bits en toAdd
			left = 8; 
			toTakeLeft -= lastTaken;// Le quita los bits que sobran a la izquierda
			flag = 1; //Bandera que activa el caso final
			*msg = toAdd;//Añade el caracter en toAdd al arreglo de mensaje
			msg++; //Mueve el apuntador del mensaje a la siguiente posicion.
			toAdd = toAdd << 8;//Lo deja en 0 (Corrimiento de 8).
		}
	}
}


/**
* Extrae n bits a partir del bit que se encuentra en la posici�n bitpos en la secuencia de bytes que
* se pasan como par�metro
* par�metro secuencia: Apuntador a una secuencia de bytes.
* par�metro n: Cantidad de bits que se desea extraer. 0 < n <= 8.
* par�metro bitpos: Posici�n del bit desde donde se extraer�n los bits. 0 <= n < 8*longitud de la secuencia
* retorno: Los n bits solicitados almacenados en los bits menos significativos de un unsigned char
*/
unsigned char sacarNbits(unsigned char secuencia[], int bitpos, int n)
{
	// DESARROLLO OPCIONAL: Puede ser util para el desarrollo de los procedimientos obligatorios.
	int index = 0;
	if (bitpos >= 8) // Si esta ubicado en otro char que no es el inicial
	{
		index = bitpos / 8;
		int toSubstract = index * 8;
		bitpos -= toSubstract;  // Valor de la posicion en el char
	}
	unsigned char answer = 0; //Variable de tipo unsigned char que contendra los bits solicitados
	unsigned char helper = 255; //Constante de ayuda para operaciones(Tamano del byte-1)
	helper = helper >> bitpos; //Realiza un corrimiento a la derecha del numero de bitpos en la variable helper
	answer = helper & secuencia[index]; 
	if (8 - bitpos < n && secuencia[index + 1] != '\0') { //Si el que sigue es el caracter de terminacion y 8 menos la posicion del bit es menor a los bits solicitados 
		helper = 255;
		int shiftLeft = 8 - (n - (8 - bitpos));//Operacion necesaria para saber el numero de movimientos a la izquierda.
		int shiftLeftans = n - (8 - bitpos);   //Operacion necesaria para saber el numero de movimientos a la izquierda de la respuesta.
		helper = helper << shiftLeft; //Blanquea a helper hacia la izquierda para juntar con answer.
		answer = answer << shiftLeftans;//Corrimiento para añadir bits faltantes del otro char.
		unsigned char temporary = helper & secuencia[index + 1]; //Saca los primeros bits del siguiente char
		temporary = temporary >> shiftLeft;// Corre temporary a la derecha para juntarlo con answer.
		answer = answer | temporary;//Lo junto con answer.
	}
	else if (8 - bitpos > n) { //Corre a la derecha los bits si los que me piden son mayores a los n bits.
		int shiftRight = 8 - (n + bitpos); 
		answer = answer >> shiftRight;
	}
	return answer;
}

// Lee un archivo en formato BMP y lo almacena en la estructura img
// NO MODIFICAR
void cargarBMP24(Imagen * imagen, char * nomArchivoEntrada) {
	// bmpDataOffset almacena la posici�n inicial de los datos de la imagen. Las otras almacenan el alto y el ancho
	// en pixeles respectivamente
	int bmpDataOffset, bmpHeight, bmpWidth;
	int y;
	int x;
	int	residuo;

	FILE *bitmapFile;
	bitmapFile = fopen(nomArchivoEntrada, "rb"); //Abre el archivo y rb significa "Abre un archivo en modo binario para lectura".
	if (bitmapFile == NULL) {
		printf("No ha sido posible cargar el archivo: %s\n", nomArchivoEntrada);
		exit(-1);
	}

	fseek(bitmapFile, 10, SEEK_SET); // 10 es la posici�n del campo "Bitmap Data Offset" del bmp	
	fread(&bmpDataOffset, sizeof(int), 1, bitmapFile);  //fread(ptr(puntero bloque memoria),size,count (numero de elementos con el tama�o), stream (puntero objetos que reciben info del canal) )

	fseek(bitmapFile, 18, SEEK_SET); // 18 es la posici�n del campo "height" del bmp
	fread(&bmpWidth, sizeof(int), 1, bitmapFile);
	bmpWidth = bmpWidth * 3; //Total de bytes a utilizar del ancho

	fseek(bitmapFile, 22, SEEK_SET); // 22 es la posici�n del campo "width" del bmp
	fread(&bmpHeight, sizeof(int), 1, bitmapFile);

	residuo = (4 - (bmpWidth) % 4) & 3; // Se debe calcular los bits residuales del bmp, que surjen al almacenar en palabras de 32 bits

	imagen->ancho = bmpWidth; // -> metodo abreviado de (*PUNTERO).ATRIBUTO
	imagen->alto = bmpHeight;
	imagen->informacion = (unsigned char *)calloc(bmpWidth * bmpHeight, sizeof(unsigned char)); //Calloc es parecido a malloc hace esto puntero=calloc(numeroElementos,tama�o de cada elemento) , y calloc inicializa todas las posiciones en 0.

	fseek(bitmapFile, bmpDataOffset, SEEK_SET); // Se ubica el puntero del archivo al comienzo de los datos

	for (y = 0; y < bmpHeight; y++) {
		for (x = 0; x < bmpWidth; x++) {
			int pos = y * bmpWidth + x;
			fread(&imagen->informacion[pos], sizeof(unsigned char), 1, bitmapFile);
		}
		fseek(bitmapFile, residuo, SEEK_CUR); // Se omite el residuo en los datos
	}
	fclose(bitmapFile);
}

// Esta funci�n se encarga de guardar una estructura de Imagen con formato de 24 bits (formato destino) en un archivo binario
// con formato BMP de Windows.
// NO MODIFICAR
void guardarBMP24(Imagen * imagen, char * nomArchivoSalida) {
	unsigned char bfType[2];
	unsigned int bfSize, bfReserved, bfOffBits, biSize, biWidth, biHeight, biCompression, biSizeImage, biXPelsPerMeter, biYPelsPerMeter, biClrUsed, biClrImportant;
	unsigned short biPlanes, biBitCount;
	FILE * archivoSalida;
	int y, x;
	int relleno = 0;

	int residuo = (4 - (imagen->ancho) % 4) & 3; // Se debe calcular los bits residuales del bmp, que quedan al forzar en palabras de 32 bits


	bfType[2];       // Tipo de Bitmap
	bfType[0] = 'B';
	bfType[1] = 'M';
	bfSize = 54 + imagen->alto * ((imagen->ancho) / 3) * sizeof(unsigned char);       // Tamanio total del archivo en bytes
	bfReserved = 0;   // Reservado para uso no especificados
	bfOffBits = 54;    // Tamanio total del encabezado
	biSize = 40;      // Tamanio del encabezado de informacion del bitmap	
	biWidth = (imagen->ancho) / 3;     // Ancho en pixeles del bitmap	
	biHeight = imagen->alto;    // Alto en pixeles del bitmap	
	biPlanes = 1;    // Numero de planos	
	biBitCount = 24;  // Bits por pixel (1,4,8,16,24 or 32)	
	biCompression = 0;   // Tipo de compresion
	biSizeImage = imagen->alto * imagen->ancho;   // Tamanio de la imagen (sin ecabezado) en bits
	biXPelsPerMeter = 2835; // Resolucion del display objetivo en coordenada x
	biYPelsPerMeter = 2835; // Resolucion del display objetivo en coordenada y
	biClrUsed = 0;       // Numero de colores usados (solo para bitmaps con paleta)	
	biClrImportant = 0;  // Numero de colores importantes (solo para bitmaps con paleta)	

	archivoSalida = fopen(nomArchivoSalida, "w+b"); // Archivo donde se va a escribir el bitmap
	if (archivoSalida == 0) {
		printf("No ha sido posible crear el archivo: %s\n", nomArchivoSalida);
		exit(-1);
	}

	fwrite(bfType, sizeof(char), 2, archivoSalida); // Se debe escribir todo el encabezado en el archivo. En total 54 bytes.
	fwrite(&bfSize, sizeof(int), 1, archivoSalida);
	fwrite(&bfReserved, sizeof(int), 1, archivoSalida);
	fwrite(&bfOffBits, sizeof(int), 1, archivoSalida);
	fwrite(&biSize, sizeof(int), 1, archivoSalida);
	fwrite(&biWidth, sizeof(int), 1, archivoSalida);
	fwrite(&biHeight, sizeof(int), 1, archivoSalida);
	fwrite(&biPlanes, sizeof(short), 1, archivoSalida);
	fwrite(&biBitCount, sizeof(short), 1, archivoSalida);
	fwrite(&biCompression, sizeof(int), 1, archivoSalida);
	fwrite(&biSizeImage, sizeof(int), 1, archivoSalida);
	fwrite(&biXPelsPerMeter, sizeof(int), 1, archivoSalida);
	fwrite(&biYPelsPerMeter, sizeof(int), 1, archivoSalida);
	fwrite(&biClrUsed, sizeof(int), 1, archivoSalida);
	fwrite(&biClrImportant, sizeof(int), 1, archivoSalida);

	// Se escriben en el archivo los datos RGB de la imagen.
	for (y = 0; y < imagen->alto; y++) {
		for (x = 0; x < imagen->ancho; x++) {
			int pos = y * imagen->ancho + x;
			fwrite(&imagen->informacion[pos], sizeof(unsigned char), 1, archivoSalida);
		}
		fwrite(&relleno, sizeof(unsigned char), residuo, archivoSalida);
	}
	fclose(archivoSalida);
}