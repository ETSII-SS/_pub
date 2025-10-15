char Saludo[20] = "Hola\n";

// Pasa a mayúsculas una cadena
char * PasaAMayusculas(char * Cadena)
{
	int i = 0;
	while (Cadena[i] != '\0') // Carácter de fin de cadena
	{
		if (Cadena[i] >= 'a' && Cadena[i] <= 'z')
			Cadena[i] -= 'a' - 'A';
		i++;
	}
	return Cadena;
}
