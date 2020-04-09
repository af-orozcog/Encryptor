// ISIS-1304 - FUNDAMENTOS DE INFRAESTRUCTURA TECNOLOGICA - PROYECTO 2 - 201910
// EL GRUPO DEBE SER DESARROLLADO EN GRUPOS DE A 3 PERSONAS MAXIMO
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


// Función que carga el bmp en la estructura Imagen
void cargarBMP24(Imagen * imagen, char * nomArchivoEntrada);

// Función que guarda el contenido de la estructura imagen en un archivo binario
void guardarBMP24(Imagen * imagen, char * nomArchivoSalida);

//Función que inserta un mensaje en la imagen usando n bits por Byte
void insertarMensaje(Imagen * img, unsigned char mensaje[], int n);

//Función que lee un mensaje de una imagen dando la longitud del mensaje y el numero de bits por byte usados
void leerMensaje(Imagen * img, unsigned char msg[], int l, int n);

//Función que saca n bits de una secuencia de caracteres a partir de una posición dada
unsigned char sacarNbits(unsigned char secuencia[], int bitpos, int n);

// Programa principal
// NO MODIFICAR
int main(int argc, char* argv[]) {

	Imagen *img = (Imagen *)malloc(sizeof(Imagen));
	char msg[10000] = "";
	char op, temp;
	int num, l, n;
	char nomArch[256] = "";

	printf("\nISIS-1304 - FUNDAMENTOS DE INFRAESTRUCTURA TECNOLOGICA - PROYECTO 2\n");

	if (argc != 5)
	{
		printf("Se ingreso un numero incorrecto de argumentos (%d) o se ingresaron de forma erronea... :(\n", argc);
		system("pause");
		return -1;
	}

	strcat(nomArch, argv[1]); //Se concatena el nombre del archivo en su variable respectiva
	op = argv[2][0]; //Se extrae la opción a realizar
	sscanf(argv[3], "%d", &num); //Se formatea el número de bits por Byte para leer o insertar

	if (op == 'w') {
		strcat(msg, argv[4]); //Se concatena la cadena a escribir en su variable, si se insertará un mensaje
	}
	else if (op == 'r') {
		sscanf(argv[4], "%d", &l); //Se formatea en l la longitud a leer de caracteres, si se leerá un mensaje
	}
	else {
		printf("Se ingreso una opcion invalida para el programa... :(\n");
		system("pause");
		return -1;
	}

	printf("Se ingresaron los %d argumentos correctamente!\n", argc);

	// Cargar los datos
	cargarBMP24(img, nomArch);

	if (op == 'w') {
		printf("\nMODO INSERTAR MENSAJE\n\n");
		printf(" - Longitud mensaje: %d bytes\n", strlen(msg));
		//msg[strlen(msg)] = '\0';

		printf(" - Insertando mensaje . . .\n");
		insertarMensaje(img, msg, num);
		printf(" - Insertado correctamente!\n");

		strcat(nomArch, "generado.bmp");
		guardarBMP24(img, nomArch);

		printf(" - Mensaje insertado y guardado exitosamente en el archivo/ruta '%s'! :D\n\n", nomArch);
	}
	else if (op == 'r') {
		printf("\nMODO LEER MENSAJE\n\n");

		for (int i = 0; i < l; i++) {
			msg[i] = 0;
		}
		msg[l] = 0;

		leerMensaje(img, msg, l, num);

		printf(" - Mensaje obtenido exitosamente! El mensaje es:\n\n\t%s\n\n", msg);
	}

	system("pause");
	return 0;
}

/**
* Inserta un mensaje, de a n bits por componente de color, en la imagen apuntada por img
* parámetro img: Apuntador a una imagen en cuyos pixeles se almacenará el mensaje.
* parámetro mensaje: Apuntador a una cadena de caracteres con el mensaje.
* parámetro n: Cantidad de bits del mensaje que se almacenarán en cada componente de color de cada pixel. 0 < n <= 8.
*/
// DESARROLLAR EN ENSAMBLADOR, *NO* SE PUEDEN USAR NOMBRES SIMBOLICOS
void insertarMensaje(Imagen * img, unsigned char mensaje[], int n)
{
	__asm {
		push ebp
		mov esp, ebp
		sub esp, 32; da el espacio para las variables tmp
		push ebx
		push ecx
		push edx; guarda los registros(para no alterarlos)
		push esi
		push edi
		mov eax, [ebp + 8]
		add eax, 8
		mov eax, [eax]; apuntador a informacion de la imagen
		mov[ebp - 12], eax; asigna a  ebp - 12 el apuntador de la informacion de la imagen
		mov eax, [ebp + 8]
		add eax, 4
		mov eax, [eax]
		mov[ebp - 8], eax; asigna a  ebp - 8 el alto de la imagen
		mov eax, [ebp + 8]
		mov eax, [eax]
		mov[ebp - 4], eax; asigna a ebp - 4 el ancho de la imagen
		mov eax, 0
		mov[ebp - 16], eax; inicializa counter en 0 (Cantidad de bits insertados)
		mov[ebp - 20], eax; inicializa index en 0 (index es el char en el que va)
		mov[ebp - 24], al; inicializa lector en 0
		mov ebx, 0; utiliza ebx para llevar la cuenta del ciclo
		forAlto :
		push ebx
			mov ebx, 0
			forAncho :
			mov al, 255
			mov[ebp - 28], al; inicializa helper en 255
			; mov eax, [ebp - 28]; inicializa eax en helper
			push ecx
			mov ecx, [ebp + 12]; obtiene el apuntador a mensaje
			mov edx, [ebp - 20]; obtiene el index
			add ecx, edx; le suma el index a la posicion del vector(o posicion de mensaje)
			mov al, [ecx]; le asigna a eax el char de la posicion mensaje[index]
			cmp al, 0; Si estoy en el caracter final
			mov eax, ecx
			pop ecx
			je break; termina el ciclo


			push edx
			add eax, 1; mensaje[index + 1]
			mov dl, [eax]
			cmp dl, 0; Si estoy en el caracter final
			pop edx
			jne continuar


			mov edx, 0; revisa que la multiplicacion no altere edx
			mov eax, [ebp - 20]; en eax pone index
			inc eax; index + 1
			push ecx; guarda ecx
			mov ecx, 8
			imul ecx; multiplica index + 1 por 8
			pop ecx; restaura el valor de ecx
			mov ecx, [ebp - 16]
			sub eax, ecx; le resto counter
			mov ecx, [ebp + 16]
			cmp eax, ecx; lo compara con n
			jge continuar
			mov eax, [ebp + 16]
			push eax; pasa n por parametro
			mov eax, [ebp - 16]
			push eax; pasa counter por parametro
			mov eax, [ebp + 12]
			push eax; pasa mensaje por parametro
			call sacarNbits; Llama la funcion
			add esp, 12; elimina los parametros enviados
			mov[ebp - 32], al; asigna a bitschar el resultado del call

			mov eax, [ebp - 20]; obtiene index.
			inc eax; index + 1
			push ecx; guarda ecx
			push edx; guarda edx
			mov edx, 0; revisa edx
			mov ecx, 8
			imul ecx; multiplica por 8 (index + 1)
			pop edx; restaura edx
			pop ecx; restaura ecx

			sub eax, [ebp - 16]; ((index + 1) * 8) - counter)
			mov edx, [ebp + 16]; a edx le asigno n
			sub edx, eax; Operaciones para(n - (((index + 1) * 8) - counter))
			movzx ecx, dl; pasar de un numero de 32 a un numero de 8, a cl le paso el numero para realizar el corrimiento
			shl[ebp - 32], cl; realiza el corrimiento y lo guarda en bitschar
			mov edx, [ebp + 16]; obtiene n
			movzx ecx, dl; a cl le paso el valor de n
			push eax
			mov al, [ebp - 28]
			shl al, cl;  Corrimiento de n bits en helper
			pop eax
			mov eax, [ebp - 12]
			mov al, [eax]; obtiene lo apuntado por image
			and al, [ebp - 28]
			mov edx, [ebp - 12];
		mov[edx], al;  modifica lo apuntado por image.RECORDAR QUE SON CHARS
			mov edx, [edx]
			mov[ebp - 24], dl; lector recibe lo apuntado por image
			mov al, [ebp - 24]; le paso el char del lector a al
			mov dl, [ebp - 32]
			or al, dl
			mov edx, [ebp - 12]
			mov[edx], al
			jmp break


			continuar:
		mov eax, [ebp + 16]
			push eax; pasa n por parametro
			mov eax, [ebp - 16]
			push eax; pasa counter por parametro
			mov eax, [ebp + 12]
			push eax; pasa mensaje por parametro
			call sacarNbits; Llama la funcion
			add esp, 12; elimina los parametros enviados
			mov[ebp - 32], al; asigna a bitschar el resultado del call

			mov edx, [ebp + 16]; obtiene n de ebp
			movzx ecx, dl; asigna a cl el numero de corrimiento(en ecx)
			push eax
			mov eax, 0
			mov al, [ebp - 28]
			shl al, cl;  Corrimiento de n bits en helper
			mov[ebp - 28], al
			pop eax
			mov eax, [ebp - 12]; obtiene *image
			mov al, [eax]; obtiene lo apuntado por image
			mov dl, [ebp - 28]; obtiene helper
			and al, dl; realiza un and entre lo apuntado por image y helper
			mov edx, [ebp - 12]; asigna a edx image
			mov[edx], al;  modifica lo apuntado por image.RECORDAR QUE SON CHARS

			mov dl, [edx]; obtiene el caracter apuntado por edx
			mov[ebp - 24], dl; lector recibe lo apuntado por image
			mov eax, 0
			mov al, [ebp - 24]; le paso el char del lector a al

			push ecx
			mov ecx, 0
			mov cl, [ebp - 32]; le mueve a cl bitschar
			or al, cl; realiza un or entre el lector y bitschar

			pop ecx
			mov edx, 0
			movzx edx, al
			mov eax, edx

			mov edx, [ebp - 12]; mueve a edx el apuntador image
			mov[edx], al; mueve lo apuntado por image al char de edx

			mov eax, [ebp - 12]; mueve a eax el apuntador img
			inc eax; incrementa el apuntador
			mov[ebp - 12], eax; refresca el apuntador img

			mov eax, [ebp - 16]; Operacion counter += n
			add eax, [ebp + 16]; Operacion counter += n
			mov[ebp - 16], eax; counter += n

			mov eax, [ebp - 16]; divide el counter por 8 Para saber en que posicion del bit voy
			push ecx
			push edx
			mov edx, 0
			mov ecx, 8
			idiv ecx; divide por 8
			pop edx
			pop ecx
			mov[ebp - 20], eax; index = counter / 8

			inc ebx; aumenta el contador del for
			cmp ebx, [ebp - 4]
			jne forAncho

			pop ebx; termina el forAncho
			inc ebx; incrementa el forAlto
			cmp ebx, [ebp - 8]; Realiza la comparacion para saber si ya termino de recorrer por Alto
			jne forAlto
			je final
			break:

		pop ebx; realiza el pop del forAlto
			final:
		pop edi; termina de hacer pop con los resgistros para restaurar su valor
			pop esi
			pop edx
			pop ecx
			pop ebx
			add esp, 32; elimina el espacio usado por parametro.
			pop ebp
			ret
	}
}

/**
* Extrae un mensaje de tamanio l, guardado de a n bits por componente de color, de la imagen apuntada por img
* parámetro img: Apuntador a una imagen que tiene almacenado el mensaje en sus pixeles.
* parámetro msg: Apuntador a una cadena de caracteres donde se depositará el mensaje.
* parámetro l: Tamanio en bytes del mensaje almacenado en la imagen.
* parámetro n: Cantidad de bits del mensaje que se almacenan en cada componente de color de cada pixel. 0 < n <= 8.
*/
// DESARROLLAR EN ENSAMBLADOR, SE PUEDEN USAR NOMBRES SIMBOLICOS
void leerMensaje(Imagen * img, unsigned char msg[], int l, int n) {
	unsigned char *image = img->informacion; //Apuntador que obtiene la direccion de informacion de la estrutura img
	int toTakeLeft = l * 8; //Cuenta de bits que faltan por leer (GLOBAL)
	int index = 1; //index+1, para saber que parametro de posicion utilizar para sacar N bits
	int left = 8; //Cuantos me faltan por tomar para tomar de un char completo
	unsigned char toAdd = 0; //Char vacio para rellenar con los bits del mensaje codificado. 
	int flag = 0; //Caso especial en que no se pueden recuperar n cantidad de bits
	int space = n; //En cuantos de los ultimos bits quedo el mensaje
	int lastTaken = 0;  //Siempre toma los ultimos bits que ingreso al mensaje
	int toTake = space - lastTaken; //Lasttaken cantidad de bits que se tomo por ultima vez.
	unsigned char valor = sacarNbits(image, (8 * index) - toTake, toTake);

	__asm {

		while:;;; while (toTakeLeft > 0)
		mov ebx, toTakeLeft
		mov eax, 0
		cmp ebx, eax
		jle finWhile

		mov ebx, flag;;;	if (flag)
		cmp ebx, eax
		jz else1Fif

		mov eax, space;;; toTake = space - lastTaken;
		mov ebx, lastTaken
			sub eax, ebx
			mov toTake, eax

			push toTake;;; valor = sacarNbits(image, (8 * index) - toTake, toTake);
		mov ebx, 8
			mov edx, 0
			mov eax, index
			imul eax, ebx
			sub eax, toTake
			push eax
			push image
			call sacarNbits
			add esp, 12
			mov valor, al

			mov al, toAdd			;;; toAdd = toAdd | valor;
		mov bl, valor
			or al, bl
			mov toAdd, al

			mov eax, toTake;;; lastTaken = toTake;
		mov lastTaken, eax

			mov eax, left;;; left -= toTake;
		sub eax, toTake
			mov left, eax

			mov eax, toTakeLeft;;; toTakeLeft -= toTake;
		sub eax, toTake
			mov toTakeLeft, eax

			inc index;;; index++;

		mov flag, 0;;; flag = 0;

		mov eax, left;;; if (left < space)
			mov ebx, space
			cmp eax, ebx
			jge elseSif

			mov al, toAdd;;; toAdd = toAdd << left;
			mov edx, left
			movzx ecx, dl; Pasar un numero de 32 a uno de 8
			shl al, cl
			mov toAdd, al

			jmp finSif

			elseSif : ;;; else

			mov al, toAdd;;; toAdd = toAdd << space;
			mov edx, space
			movzx ecx, dl; Pasar un numero de 32 a uno de 8
			shl al, cl
			mov toAdd, al

			finSif :

		jmp finFif

			else1Fif :
		mov eax, toTakeLeft;;; else if (toTakeLeft < space)
			mov ebx, space
			cmp eax, ebx
			jge else2Fif

			push toTakeLeft;;; valor = sacarNbits(image, (8 * index) - space, toTakeLeft);
		mov ebx, 8
			mov edx, 0
			mov eax, index
			imul eax, ebx
			sub eax, space
			push eax
			push image
			call sacarNbits
			add esp, 12
			mov valor, al

			mov al, toAdd;;; toAdd = toAdd | valor;
		mov bl, valor
			or al, bl
			mov toAdd, al
			
			mov ebx, msg
			mov [ebx], al	;;; *msg = toAdd;

		inc msg;;; msg++;

		mov eax, 0;;; toTakeLeft = 0;
		mov toTakeLeft, eax

			jmp finFif


			else2Fif : ;;; else if (left >= space)
			mov eax, left
			mov ebx, space
			cmp eax, ebx
			jl else3Fif

			push space;;; valor = sacarNbits(image, (8 * index) - space, space);
		mov ebx, 8
			mov edx, 0
			mov eax, index
			imul eax, ebx
			sub eax, space
			push eax
			push image
			call sacarNbits
			add esp, 12
			mov valor, al

			mov al, toAdd;;; toAdd = toAdd | valor;
		mov bl, valor
			or al, bl
			mov toAdd, al

			mov eax, space;;; lastTaken = space;
		mov lastTaken, eax

			mov eax, left;;; left -= space;
		sub eax, space
			mov left, eax

			mov eax, toTakeLeft;;; toTakeLeft -= space;
		sub eax, space
			mov toTakeLeft, eax

			mov eax, left;;; if (left == 0)
			mov ebx, 0
			cmp eax, ebx
			jnz else1Tif

			mov ebx, msg
			mov al, toAdd
			mov [ebx], al;;; *msg = toAdd;

		inc msg;;; msg++;

		mov eax, 0;;; toAdd = 0;
		mov toAdd, al

			mov eax, 8;;; left = 8;
		mov left, eax

			jmp finTif

			else1Tif : ;;; else if (left < space)
			mov eax, left
			mov ebx, space
			cmp eax, ebx
			jge else2Tif

			mov al, toAdd;;; toAdd = toAdd << left;
			mov edx, left
			movzx ecx, dl; Pasar un numero de 32 a uno de 8
			shl al, cl
			mov toAdd, al

			jmp finTif

			else2Tif:		;;; else

			mov al, toAdd;;; toAdd = toAdd << space;
			mov edx, space
			movzx ecx, dl; Pasar un numero de 32 a uno de 8
			shl al, cl
			mov toAdd, al

			finTif :

		inc index

			else3Fif : ;;; else if (left < space)
			mov eax, left
			mov ebx, space
			cmp eax, ebx
			jge finFif

			mov eax, left;;; lastTaken = left;
		mov lastTaken, eax

			push left;;; valor = sacarNbits(image, (8 * index) - space, left);
		mov ebx, 8
			mov edx, 0
			mov eax, index
			imul eax, ebx
			sub eax, space
			push eax
			push image
			call sacarNbits
			add esp, 12
			mov valor, al

			mov al, toAdd;;; toAdd = toAdd | valor;
		mov bl, valor
			or al, bl
			mov toAdd, al

			mov eax, 8;;; left = 8;
		mov left, eax

			mov eax, toTakeLeft;;; toTakeLeft -= lastTaken;
		sub eax, lastTaken
			mov toTakeLeft, eax

			mov eax, 1;;; flag = 1;
		mov flag, eax

			mov ebx, msg
			mov al, toAdd
			mov [ebx], al;;; *msg = toAdd;

		inc msg;;; msg++;

		mov al, toAdd;;; toAdd = toAdd << 8;
		shl al, 8
			mov toAdd, al

			finFif :

		jmp while

			finWhile :
	}
}

/**
* Extrae n bits a partir del bit que se encuentra en la posición bitpos en la secuencia de bytes que
* se pasan como parámetro
* parámetro secuencia: Apuntador a una secuencia de bytes.
* parámetro n: Cantidad de bits que se desea extraer. 0 < n <= 8.
* parámetro bitpos: Posición del bit desde donde se extraerán los bits. 0 <= n < 8*longitud de la secuencia
* retorno: Los n bits solicitados almacenados en los bits menos significativos de un unsigned char
*/
// DESARROLLAR EN ENSAMBLADOR, SE PUEDEN USAR NOMBRES SIMBOLICOS
unsigned char sacarNbits(unsigned char secuencia[], int bitpos, int n)
{
	// DESARROLLO OPCIONAL: Puede ser útil para el desarrollo de los procedimientos obligatorios.
	__asm {
		push ebp
		mov esp, ebp
		sub esp, 12
		push edx
		push ecx
		push ebx
		push esi
		push edi
		mov eax, 0
		mov[ebp - 4], eax
		mov[ebp - 8], al
		mov eax, 0
		mov al, 255
		mov[ebp - 12], al
		mov ecx, [ebp - 4]

		push eax
		mov eax, [ebp + 12]
		cmp eax, 8
		pop eax
		jl intermedio
		mov eax, bitpos; index = bitpos / 8
		mov edx, 0
		mov ecx, 8
		idiv ecx
		mov[ebp - 4], eax; int toSubstract = index * 8
		mov eax, [ebp - 4]; se recupera lo de index - int toSubstract = index * 8
		mov edx, 0;
		imul ecx; se multiplica el index por ocho.
			mov edx, eax; se copia el resultado de la multiplicación de index * 8
			sub bitpos, edx; bitpos -= toSubstract
			intermedio :
		mov dl, [ebp - 12]; helper = helper >> bitpos
			mov eax, bitpos
			movzx ecx, al; Pasar un numero de 32 a uno de 8
			shr dl, cl;
		mov[ebp - 12], dl; se pega en la variable el resultado que se acaba de
			mov dl, [ebp - 12]; answer = helper & secuencia[index]
			mov ecx, [ebp - 4]; se mueve index a ecx
			mov ebx, secuencia
			add ebx, ecx
			push ebx
			mov bl, [ebx]
			and dl, bl; se hace el and logico
			pop ebx
			mov[ebp - 8], edx; se copia el valor obtenido a la variable answer
			mov edx, 8; 8 - bitpos < n && secuencia[index + 1] != '\0'
			sub edx, bitpos; se resta para hacer la primera comparación
			cmp edx, n; se hace la comparación con n
			jge tercerIf
			mov edx, [ebp - 4]; se mueve index
			inc edx; se incrementa en 1 edx
			cmp secuencia[edx], 0
			je tercerIf
			segundoIf :

		mov al, 255;
		mov[ebp - 12], al; helper = 255
			mov edx, 8; int shiftLeft = 8 - (n - (8 - bitpos))
			sub edx, bitpos; se hace la primera resta del parentesis
			mov eax, n; en eax va a quedar shiftleftans
			sub eax, edx; int shiftLeftans = n - (8 - bitpos)
			mov ecx, n; se guarda n
			sub ecx, edx; se hace n - (8 - bitpos)
			mov edx, 8; se pone 8 en edx
			sub edx, ecx; en edx va a quedar shiftleft
			movzx ecx, dl; pasar de un numero de 32 a un numero de 8
			push eax
			mov al, [ebp - 12]
			shl al, cl; helper = helper << shiftLeft
			pop eax
			movzx ecx, al
			push eax
			mov al, [ebp - 8]
			shl al, cl; answer = answer << shiftLeftans
			mov[ebp - 8], al
			pop eax
			mov bl, [ebp - 12]; unsigned char temporary = helper & secuencia[index + 1]
			mov eax, [ebp - 4]; se guarda index
			inc eax; se incrementa eax
			mov ecx, secuencia
			add ecx, eax
			and bl, [ecx]; se hace el y logico que se guarda en dl
			movzx ecx, dl
			shr bl, cl; se corre hacia la derecha el dl temporary = temporary >> shiftLeft
			or [ebp - 8], bl; answer = answer | temporary;
		jmp fin
			tercerIf :
		mov eax, 8; se mueve a eax el 8
			sub eax, bitpos; se resta a 8 bitpos
			cmp eax, n; (8 - bitpos > n)
			jle fin
			mov eax, 8;
		mov ebx, n; se pasa a ebx la n
			add ebx, bitpos; se suma ebx con bitpos
			sub eax, ebx; 8 - (n + bitpos)
			movzx ecx, al
			shr[ebp - 8], cl; answer >> shiftRight
			fin :
		pop ebx
			pop edi
			pop esi
			pop ecx
			pop edx
			mov eax, 0
			movzx eax, [ebp - 8]; se pone la respuesta
			add esp, 12; se vacian las variables locales
			pop ebp
			ret
	}
}

// Lee un archivo en formato BMP y lo almacena en la estructura img
// NO MODIFICAR
void cargarBMP24(Imagen * imagen, char * nomArchivoEntrada) {
	// bmpDataOffset almacena la posición inicial de los datos de la imagen. Las otras almacenan el alto y el ancho
	// en pixeles respectivamente
	int bmpDataOffset, bmpHeight, bmpWidth;
	int y;
	int x;
	int	residuo;

	FILE *bitmapFile;
	bitmapFile = fopen(nomArchivoEntrada, "rb");
	if (bitmapFile == NULL) {
		printf("No ha sido posible cargar el archivo: %s\n", nomArchivoEntrada);
		exit(-1);
	}

	fseek(bitmapFile, 10, SEEK_SET); // 10 es la posición del campo "Bitmap Data Offset" del bmp	
	fread(&bmpDataOffset, sizeof(int), 1, bitmapFile);

	fseek(bitmapFile, 18, SEEK_SET); // 18 es la posición del campo "height" del bmp
	fread(&bmpWidth, sizeof(int), 1, bitmapFile);
	bmpWidth = bmpWidth * 3;

	fseek(bitmapFile, 22, SEEK_SET); // 22 es la posición del campo "width" del bmp
	fread(&bmpHeight, sizeof(int), 1, bitmapFile);

	residuo = (4 - (bmpWidth) % 4) & 3; // Se debe calcular los bits residuales del bmp, que surjen al almacenar en palabras de 32 bits

	imagen->ancho = bmpWidth;
	imagen->alto = bmpHeight;
	imagen->informacion = (unsigned char *)calloc(bmpWidth * bmpHeight, sizeof(unsigned char));

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

// Esta función se encarga de guardar una estructura de Imagen con formato de 24 bits (formato destino) en un archivo binario
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