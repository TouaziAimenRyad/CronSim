#include "../include/includes.h"
#include "../include/command.h"

// Structure chaine 
typedef struct{
    int taille;
	char* ptr;
}Chaine;
 
// Structure tableau 

typedef struct{
	int nbChaine;
    Chaine *chaine;

}Tableau;
 

// Création d'une copie de la chaine ch : 

Chaine copierChaine(Chaine ch)
{
	// On crée une nouvelle chaine à partir de l'autre pour pouvoir la manipuler
    Chaine c = { ch.taille, malloc(sizeof(char) * ch.taille) }; 
	// On copie les caractères
    strcpy(c.ptr, ch.ptr); 
    // et on retourne la nouvelle chaine c 
    return c;
}

// Céation d'un tableau de chaines :

Tableau creerTableauChaine(Chaine c)
{
	// On crée un tableau, on alloue la place nécessaire
    Tableau t = { 1, malloc(sizeof(Chaine)) };
	char *p = strtok(c.ptr, " ");
	if(p != NULL){
	Chaine d = {strlen(p),p};
	// On met notre première chaine dans la première case de notre tableau
    t.chaine[0] = copierChaine(d);
	}
	return(t);
}
 
Tableau ajouterChaine(Chaine chaine, Tableau tab)
{
	//  On réalloue un pointeur
    Chaine* r = realloc(tab.chaine, tab.nbChaine + 1);

	//  On vérifie si la réallocation a été bien faite 
    if (r == NULL) {
	Tableau tab = {0,NULL};
        return tab;
	}
	//  On change le pointeur
    tab.chaine = r;
    tab.nbChaine++;
    tab.chaine[tab.nbChaine - 1] = copierChaine(chaine);
	return(tab);
}

// PROBLEM IN THE FONCTION IDK WHY :3
// If one of u arrive à la corriger ! 
/*
void getwords(Chaine str){

	char *ptr = strtok(str.ptr, " ");
	Chaine v = copierChaine(str);
	Tableau t = creerTableauChaine(v);

	while(ptr != NULL){
	Chaine d = {strlen(ptr),ptr};
	ajouterChaine(d,t);
    ptr = strtok( NULL, " ");
	}
	// return(ptr.(t.chaine));
	
}*/


uint16_t convert_char_to_uint16(const char *str){
  char *end;
  errno = 0;
  intmax_t val = strtoimax(str, &end, 10);
  if (errno == ERANGE || val < 0 || val > UINT16_MAX || end == str || *end != '\0'){
    printf(" can't do the conversion\n");
	return(0);
  }
  return (uint16_t) val;
}



uint32_t convert_char_to_uint32(char str []) 
{
	uint32_t result = (str[0] << 24 | str[1] << 16 | str[2] << 8 | str[3]);
    return result;

}


// uint6_t convert_char_to_uint64(char *str){
// 
// }
// 
// 
// uint32_t * convert_str_to_ARGV(char * str){
// }