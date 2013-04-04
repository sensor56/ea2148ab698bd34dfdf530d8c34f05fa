// Supports PDF d'Ateliers Arduino par www.mon-club-elec.fr
// http://www.mon-club-elec.fr/pmwiki_mon_club_elec/pmwiki.php?n=MAIN.ATELIERS
// par X. HINAULT - tous droits réservés - 2013 - GPLv3

// code : 26. Programme d'exemple SPI : communiquer avec une carte mémoire SD 

#include <SD.h>
#include <SPI.h>

File myFile;
Sd2Card card;
SdVolume volume;

void setup()
{
  Serial.begin(115200); // utiliser le meme debit coté Terminal Serie
  Serial.println("Initialisation de la SD card...");

  pinMode(10, OUTPUT); // laisser la broche SS en sortie - obligatoire avec librairie SD

  //if (!SD.begin(10)) { // si initialisation avec broche 4 en tant que CS n'est pas réussie
 if (!card.init(SPI_HALF_SPEED, 10)) {

    Serial.println("Echec initialisation!"); // message port Série
    return; // sort de setup()
  }


  Serial.println("Initialisation reussie !"); // message port Série

 // affiche le type de la carte 
  Serial.print("Type de carte SD : ");
  switch(card.type()) {
    case SD_CARD_TYPE_SD1:
      Serial.println("SD1");
      break;
    case SD_CARD_TYPE_SD2:
      Serial.println("SD2");
      break;
    case SD_CARD_TYPE_SDHC:
      Serial.println("SDHC");
      break;
    default:
      Serial.println("Unknown");
  }

  // Essai ouverture partition qui doit etre FAT16 or FAT32
  if (!volume.init(card)) {
    Serial.println("Pas de partition FAT16/FAT32. Reformater la carte SD !");
    return;
  }


  // affiche la taille de la première partition 
  uint32_t volumesize;
  Serial.print("Partition de type FAT");
  Serial.println(volume.fatType(), DEC);
  Serial.println();
  
} // fin setup 

void loop(){ // debut de la fonction loop()



} // fin de la fonction loop() 



