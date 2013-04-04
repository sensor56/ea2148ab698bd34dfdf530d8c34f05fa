// Supports PDF d'Ateliers Arduino par www.mon-club-elec.fr
// http://www.mon-club-elec.fr/pmwiki_mon_club_elec/pmwiki.php?n=MAIN.ATELIERS
// par X. HINAULT - tous droits réservés - 2013 - GPLv3

// code : 18. Exemple d'utilisation SPI : Communiquer directement avec une mémoire Eeprom SPI 

// --- Inclusion des librairies ---
#include <SPI.h> // Librairie pour communication SPI


// --- Déclaration des constantes utiles ---

// --- les codes des instructions de l'eeprom SPI
const int WREN=B110; // code instruction Write Enable
const int WRDI=B100; // code instruction Write Reset
const int RDSR=B101; // code instruction Read Status Register
const int WRSR=B001; // code instruction Write Status Register
const int READ=B011; // code instruction Read Data from memory
const int WRITE=B010; // code instruction Write Data to memory


// --- Déclaration des constantes des broches E/S numériques ---

//const int DATAOUT=11; //MOSI
//const int DATAIN=12; //MISO
//const int SPICLOCK=13; //sck
const int chipSelectPin=10; //ss (Slave Select)


// --- Déclaration des constantes des broches analogiques ---


// --- Déclaration des variables globales ---

unsigned int adresse=2000; // adresse 16 bits
byte adresseMSB, adresseLSB; // variables pour adresse
byte data=0;
long memoMillis=0; // pour mémoriser valeur renvoyée par millis

void setup()   { // debut de la fonction setup()

Serial.begin(115200); // initialise connexion série à 115200 bauds
// IMPORTANT : régler le terminal côté PC avec la même valeur de transmission

// initialisation de la librairie SPI 
  SPI.begin();  // Arduino configuré en maitre 

  //-------- sens bits ----
  SPI.setBitOrder(MSBFIRST); // fixe le sens de la communication

  //------- Configuration Mode (Impulsion horloge inactive  et Front validation données
  //   Mode        Horloge inactive    Front validation donnée
  //   SPI_MODE0   0                    0
  //   SPI_MODE1   0                    1
  //   SPI_MODE2   1                    0  
  //   SPI_MODE3   1                    1  
  SPI.setDataMode(SPI_MODE0); // l'eeprom AT25256 fonctionne en mode 0

  //-------- configuration de l'horloge ----
  SPI.setClockDivider(SPI_CLOCK_DIV4); // le plus rapide

//---- met en sortie la broche de sélection de l'eeprom
  pinMode(chipSelectPin, OUTPUT);

} // fin de la fonction setup()

void loop(){ // debut de la fonction loop()

// activation écriture 
  digitalWrite(chipSelectPin, LOW); // sélection l'eeprom
  SPI.transfer(WREN); // active l'écriture - à faire au début séquence écriture
  digitalWrite(chipSelectPin, HIGH); // dé-sélectionne l'eeprom

   delay(10); // pause

//----- message debug
memoMillis=millis(); // mémorise valeur millis()
Serial.print("Millis debut ecriture: "),Serial.println(memoMillis);


// ---- écriture donnée dans l'EEPROM
    digitalWrite(chipSelectPin, LOW); // sélection l'eeprom

    SPI.transfer(WRITE); // instruction écriture

    //adresse=70;
    adresseMSB=adresse>>8; // isole les 8 bits forts
    adresseLSB= adresse; // isole les 8 bits faibles

    SPI.transfer(adresseMSB); // envoie MSB adresse en 1er
    SPI.transfer(adresseLSB); // envoie LSB adresse  

    data=data+1; // donnée à écrire valeur 0 - 255
    SPI.transfer(data); // écriture donnée eeprom

    digitalWrite(chipSelectPin, HIGH); // dé-sélectionne l'eeprom - obligatoire pour lancer l'écriture


    //---- teste la fin du cycle écriture ---
    digitalWrite(chipSelectPin, LOW); // sélection l'eeprom

    while(bitRead(SPI.transfer(RDSR),0)!=0); // attend que le bit Ready du registre statut passe à 0

    digitalWrite(chipSelectPin, HIGH); // dé-sélectionne l'eeprom

//----- message debug
memoMillis=millis(); // mémorise valeur millis()
Serial.print("Millis fin ecriture: "),Serial.println(memoMillis);

   delay(5); // pause pour laisser temps eeprom travailler

//---- lecture donnée eeprom
  digitalWrite(chipSelectPin, LOW); // sélection l'eeprom
  SPI.transfer(READ); // active la lecture

    //adresse=70;
    adresseMSB=adresse>>8; // isole les 8 bits forts
    adresseLSB= adresse; // isole les 8 bits faibles

    SPI.transfer(adresseMSB); // envoie MSB adresse en 1er
    SPI.transfer(adresseLSB); // envoie LSB adresse  

  data=0;
  data=SPI.transfer(0xFF); // lecture de la donnée...
  // pourquoi 0xFF ? en fait la fonction attend une valeur qui peut etre quelconque
  // NB : avant écriture, les emplacements mémoires valent 0xFF

  digitalWrite(chipSelectPin, HIGH); // dé-sélectionne l'eeprom

  Serial.print("Valeur lue en Eeprom : "),Serial.println(data, DEC);

  //while(1);
  delay(1000); // entre 2 passages


} // fin de la fonction loop()
