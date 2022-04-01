#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <locale.h>




int maps[30][100];
char playerName[30];
struct player realPlayer;
struct position playerPos;
struct player *rivals;
int rivalNum;
int dotCount=0;
int clrCount=0;
struct position *posPtr;
int posInt;


struct position{    // Oyuncu koordinatý
    int x;
    int y;
}deneme;
struct items{   	// Oyuncu item lari
    int playerShields;
    int playerScouts;
    int playerMoves;
};
struct resource{	// Kaynaklar
	int food;
	int wood;
	int stone;
	int gold;
	int iron;
};
struct infantry{    //Piyade
    int power;
    int amount;
    int level;
};
struct cavalry{     //Sovalye
    int power;
    int amount;
    int level;
};
struct archer{      //Okcu
    int power;
    int amount;
    int level;
};
struct siege{       //Kusatma
    int power;
    int amount;
    int level;
};
struct troops{		//Birlikler
    struct infantry playerInfantry;
    struct cavalry playerCavalry;
    struct archer playerArcher;
    struct siege playerSiege;
};
struct player{      // Oyuncu yapýsý      
    int id;
    char *playerName;
    struct position playerPosition;
    struct items playerItems; 
    struct troops playerTroops; // Birlikler
	struct resource playerResource;

};
struct warResult{   // Savas sonucunda oyuncularýn guncel yapýlarýný tutan yapi
    struct player attacker;
    struct player defender;
};




int getTotalPower(struct player p){						// Int olarak toplam askeri gucu alma fonksiyonu
	
	struct player temp = p;
	int tempInfTroop = temp.playerTroops.playerInfantry.power;
    int tempCavalryTroop = temp.playerTroops.playerCavalry.power;
    int tempArcherTroop = temp.playerTroops.playerArcher.power;
    int tempSiegeTroop = temp.playerTroops.playerSiege.power;
    int tempTotalPower = tempInfTroop + tempCavalryTroop + tempArcherTroop + tempSiegeTroop;
    
    return tempTotalPower;
}
int *getPowers(struct player p){						// Referans ile çaðýrarak oyuncunun askeri güclerini dizi ile aldýk
	
	struct player temp = p;
	int tempInfTroop = temp.playerTroops.playerInfantry.power;
    int tempCavalryTroop = temp.playerTroops.playerCavalry.power;
    int tempArcherTroop = temp.playerTroops.playerArcher.power;
    int tempSiegeTroop = temp.playerTroops.playerSiege.power;
    
    int powersArry[4] = {tempInfTroop,tempCavalryTroop,tempArcherTroop,tempSiegeTroop};
    
    int *powers = powersArry;
    
    return powers;
}
void calculateUnitPowers(struct troops *pTroops){		// Savaþ ya da asker eðitme fonksiyonlarýnda bunu çaðýrýp oyuncu askeri güçlerini güncelliyoruz
	pTroops->playerInfantry.power = ((pTroops->playerInfantry.amount)*((pTroops->playerInfantry.level)*(5)));
	pTroops->playerCavalry.power = ((pTroops->playerCavalry.amount)*((pTroops->playerCavalry.level)*(5)));
	pTroops->playerArcher.power = ((pTroops->playerArcher.amount)*((pTroops->playerArcher.level)*(5)));
	pTroops->playerSiege.power = ((pTroops->playerSiege.amount)*((pTroops->playerSiege.level)*(5)));
}




//Bu savaþ fonksiyonlarý düzenlenecek		---	????
void startWar(struct player *attacker, struct player *defender){
        
    int attackerTotalPower = getTotalPower(*attacker);
    int defenderTotalPower = getTotalPower(*defender);
    
    if(attackerTotalPower > defenderTotalPower){
    	
        defender->playerTroops.playerInfantry.amount -= 5;
        defender->playerTroops.playerCavalry.amount -= 5;
        defender->playerTroops.playerArcher.amount -= 5;
        defender->playerTroops.playerSiege.amount -= 5; 
		calculateUnitPowers(&defender->playerTroops);   
        
        printf("\nÞehir Yaðmalandý!\nElde Edilen Ganimetler:\n");
        printf("\nYiyecek	:	%d", (defender->playerResource.food/2));
        printf("\nOdun	:	%d", (defender->playerResource.wood/2));
        printf("\nTaþ	:	%d", (defender->playerResource.stone/2));
        printf("\nAltýn	:	%d", (defender->playerResource.gold/2));
        printf("\nDemir	:	%d", (defender->playerResource.iron/2));
        
        attacker->playerResource.food += (defender->playerResource.food/2);
        defender->playerResource.food -= (defender->playerResource.food/2);
        
        attacker->playerResource.wood += (defender->playerResource.wood/2);
        defender->playerResource.wood -= (defender->playerResource.wood/2);
        
        attacker->playerResource.stone += (defender->playerResource.stone/2);
        defender->playerResource.stone -= (defender->playerResource.stone/2);
        
        attacker->playerResource.gold += (defender->playerResource.gold/2);
        defender->playerResource.gold -= (defender->playerResource.gold/2);
        
        attacker->playerResource.iron += (defender->playerResource.iron/2);
        defender->playerResource.iron -= (defender->playerResource.iron/2);  
    }else{
    	int* beforeFromWar = (int*) malloc(4 * sizeof(int));
		int i;
		for(i =0; i<4; i++){
			beforeFromWar[i] = getPowers(*attacker)[i];
		}
        attacker->playerTroops.playerInfantry.amount-= 2;
        attacker->playerTroops.playerCavalry.amount-= 2;
        attacker->playerTroops.playerArcher.amount-= 1;
        attacker->playerTroops.playerSiege.amount-= 5;
        calculateUnitPowers(&attacker->playerTroops);
        
        printf("\nKaybedilen Asker Ýstatistikleri:");
        printf("\nPiyade  : %d", 2);
        printf("\nSüvari  : %d", 2);
        printf("\nOkçu  : %d", 1);
        printf("\nKuþatma  : %d", 5);
        int* afterFromWar = (int*) malloc(4 * sizeof(int));
		int j;
		printf("\n\nAskeri Kayýplar Sonrasý Kalan Kýta Güçleri:");
		char *troopsNames[4] = {"Piyade", "Süvari", "Okçu", "Kuþatma"};
		for(j =0; j<4; j++){
			afterFromWar[j] = getPowers(*attacker)[j];
			printf("\n%s : %d", troopsNames[j], afterFromWar[j]);
		}
        attacker->playerResource.food -= (attacker->playerResource.food/10);
        attacker->playerResource.wood -= (attacker->playerResource.wood/10);
        attacker->playerResource.stone -= (attacker->playerResource.stone/10);
        attacker->playerResource.gold -= (attacker->playerResource.gold/10);
        attacker->playerResource.iron -= (attacker->playerResource.iron/10);
        
    }
}
struct player *findPlayerFromMap(int x, int y){
	
	struct player *found;
	int i;
	for(i = 0; i < 10; i++){
		if(rivals[i].playerPosition.x == x && rivals[i].playerPosition.y == y){
			found = &rivals[i];
		}
	}
	
	return found;
}
void warMenu(){
	
	int x,y;
	printf("Saldýrmak istediðiniz oyuncunun konumunu giriniz [0-30][0-100] : ");
	scanf("%d %d", &x,&y);
	startWar(&realPlayer, findPlayerFromMap(x, y));
	
}



void trainTroops(struct player *playerPtr){						// Referans ile çaðýrarak asker eðittik
       
    int powerBefore = getTotalPower(*playerPtr);
	   
    playerPtr->playerTroops.playerInfantry.amount += 10;    
    playerPtr->playerTroops.playerCavalry.amount += 10;
    playerPtr->playerTroops.playerArcher.amount += 10;
	playerPtr->playerTroops.playerSiege.amount += 10;
	
    calculateUnitPowers(&playerPtr->playerTroops);
    
    int powerAfter = getTotalPower(*playerPtr);
    printf("\nMevcut Askeri Güç : %d", powerBefore);
    printMessage("\n\nAskerler Eðitiliyor.");
    printf("\nBirliðe \n10 Piyade\n10 Süvari\n10 Okçu\n10 Kuþatma\nToplamda 40 asker kazandýrýldý.");
    printf("\nYeni Katýlan Askeri Güç : %d", (powerAfter-powerBefore));
    printf("\nUlaþýlan Toplam Askeri Güç : %d", powerAfter);
    //printf("\nOyuncunun Güncel Verileri");
    //printPlayer(realPlayer);
}
struct player createPlayer(char *Name, int xPos, int yPos){		// Oyuncu oluþturma
    
    static int playerID = 1;
    
    //Player yapýsýný oluþturalým
    struct player newPlayer;
    
    //player Birliklerinin varsayýlan degerlerini oluþturalým
    struct troops newPlayerTroops;
    
    newPlayerTroops.playerInfantry.amount = 50;
    newPlayerTroops.playerInfantry.level = 1;
    newPlayerTroops.playerInfantry.power = ((newPlayerTroops.playerInfantry.amount)*((newPlayerTroops.playerInfantry.level)*(5)));
    
    newPlayerTroops.playerCavalry.amount = 50;
    newPlayerTroops.playerCavalry.level = 1;
    newPlayerTroops.playerCavalry.power = ((newPlayerTroops.playerCavalry.amount)*((newPlayerTroops.playerCavalry.level)*(5)));
     
    newPlayerTroops.playerArcher.amount = 50;
    newPlayerTroops.playerArcher.level = 1;
    newPlayerTroops.playerArcher.power = ((newPlayerTroops.playerArcher.amount)*((newPlayerTroops.playerArcher.level)*(5)));
    
    newPlayerTroops.playerSiege.amount = 10;
    newPlayerTroops.playerSiege.level = 1;
    newPlayerTroops.playerSiege.power = ((newPlayerTroops.playerSiege.amount)*((newPlayerTroops.playerSiege.level)*(5)));
    
    
    //newPlayer yapimizin degerlerini atayalým
    newPlayer.id = playerID;
    newPlayer.playerName = Name;
    newPlayer.playerPosition.x = xPos;
    newPlayer.playerPosition.y = yPos;
    newPlayer.playerItems.playerShields = 2;
    newPlayer.playerItems.playerScouts = 1;
    newPlayer.playerItems.playerMoves = 2;
    newPlayer.playerTroops = newPlayerTroops;
    
    newPlayer.playerResource.wood = 100;
    newPlayer.playerResource.food = 100;
    newPlayer.playerResource.stone = 100;
    newPlayer.playerResource.gold = 100;
    newPlayer.playerResource.iron = 100;
    
    playerID++;
    return newPlayer;
}
void printPlayer(struct player p){								// Oyuncu Bilgileri yazdýrma
	
	struct player temp = p;
	
	int *powers = (int*) malloc(4 * sizeof(int));
	int i;
	for(i =0; i<4; i++){
		powers[i] = getPowers(temp)[i];
	}
	
	printf("\n___________________________________________________________________________________________________________________________________");
	printf("\n\nID	|	PLAYER NAME	|	POSITION	|ITEMS		|	RESOURCE	|	TROOPS ");
	printf("\n\n%d	|	%s		|	(%d,%d)		|Shield	: %d	|	Food	: %d	|	Infantry: %d ", temp.id, temp.playerName, temp.playerPosition.x, temp.playerPosition.y, temp.playerItems.playerShields, temp.playerResource.food, *powers);
	printf("\n\n	|			|			|Scout	: %d	|	Wood	: %d	|	Cavalry	: %d ",  temp.playerItems.playerScouts, temp.playerResource.wood, *(powers+1));
	printf("\n\n	|			|			|Move	: %d	|	Stone	: %d	|	Archer	: %d ",  temp.playerItems.playerMoves, temp.playerResource.stone, *(powers+2));
	printf("\n\n	|			|			|		|	Gold	: %d	|	Siege	: %d ",  temp.playerResource.gold, *(powers+3));
	printf("\n\n	|			|			|		|	Iron	: %d	|	Total	: %d ", temp.playerResource.iron, getTotalPower(p));
	printf("\n___________________________________________________________________________________________________________________________________");
}	
void createMap(int maps[30][100]){								// Harita oluþturma Tamamlandý 
		
	int k,l;
	for(k=0; k<30; k++){
		for(l=0; l<100; l++){
			maps[k][l] = 0;
		}
	}
}
void addPlayerToMap(int maps[30][100], struct player p){		// Haritaya oyuncu ekleme fonksiyonu
	
	maps[p.playerPosition.x][p.playerPosition.y] = (64+p.id);
	
}
void printMap(int maps[30][100]){								// Haritayý yazdýrma fonksiyonu
	
	int i,j;
	for(i = 0; i<30; i++){
		
		if(i<10){
			printf(" %d ",i);
		}else{
			printf("%d ",i);
		}
		
		for(j=0; j<100; j++){
			
			if(i==0){
				if(j < 10){
					printf("%d",j);
				}else{
					printf("%d", (j%10));
				}
				
			}else{
				
				if((maps[i][j]==0)){
					printf("+");
				}else{
					printf("%c", maps[i][j]);
				}
			}
			

		}
		printf("\n");
	}	
}







void addResources(int maps[30][100]){			// Kaynaklar haritaya Ekleniyor
    int i;
    
    for(i=0; i<10; i++){
        int randX = rand()%30;
        int randY = rand()%100;
        if(maps[randX][randY] == 0){
            maps[randX][randY] = 102;    //  lower f char for food resource
        }
    }
    for(i=0; i<10; i++){
        int randX = rand()%30;
        int randY = rand()%100;
        if(maps[randX][randY] == 0){
            maps[randX][randY] = 119;    //  lower w char for wood resource
        }
    }
    for(i=0; i<10; i++){
        int randX = rand()%30;
        int randY = rand()%100;
        if(maps[randX][randY] == 0){
            maps[randX][randY] = 115;   //  lower s char for stone resource
        }
    }
    for(i=0; i<10; i++){
        int randX = rand()%30;
        int randY = rand()%100;
        if(maps[randX][randY] == 0){
            maps[randX][randY] = 103;   //  lower g char for gold resource 
        }
    }
    for(i=0; i<10; i++){
        int randX = rand()%30;
        int randY = rand()%100;
        if(maps[randX][randY] == 0){
            maps[randX][randY] = 105;    //  lower i char for iron resource
        }
    }
}

void *addDotThread(void *vargp){				// Multithreading ile mesaja 1 saniye bekleterek yükleniyor noktasý ekleme
	sleep(1);
	printf(".");
	return NULL;
}
void addDot(){									// Noktayý ekleyen döngü
	dotCount++;
	pthread_t thread_id;
	pthread_create(&thread_id, NULL, addDotThread, NULL);
	pthread_join(thread_id, NULL);
	if(dotCount<5){
		addDot();
	}
}
void printMessage(char *message){				// Noktalý mesaj yazdirma
   printf("%s",message);
   addDot();
   dotCount = 0;
}








//Burasý kullanýlmadý tam istenen sonucu vermiyor
void *refreshScreenThread(void *vargp){
	sleep(1);
	system("cls");
	printMap(maps);
	return NULL;
}
void refreshIt(){	
	pthread_t thread_id;
	pthread_create(&thread_id, NULL, refreshScreenThread, NULL);
	pthread_join(thread_id, NULL);
}
void *waitThread(void *vargp){
	sleep(1);
	return NULL;
}
void wait(){
	pthread_t thread_id;
	pthread_create(&thread_id, NULL, waitThread, NULL);
	pthread_join(thread_id, NULL);
}

// Dizi döndürüyor ama dönen dizinin boyutunu alma yapýlacak !!!!  Ayný Satýr ya da sütun hattýndaki yolu þuan almýyor güncellenecek
void roadMap(struct position current, struct position target){
    
    int xFark, yFark, adimSayisi;
    int xYon = current.x - target.x;
    int yYon = current.y - target.y;
    
    
	//struct position *posPtr;
    
    
    if(current.x >= target.x){
        xFark = current.x - target.x;
    }else{
        xFark = target.x - current.x;
    }
    
    if(current.y >= target.y){
        yFark = current.y - target.y;
    }else{
        yFark = target.y - current.y;
    }
    
    if(xFark > yFark){
        adimSayisi = xFark/yFark;
        //posInt = (xFark-(adimSayisi+2));
        posInt = xFark;
        posPtr = calloc(posInt, sizeof(struct position));
        
        
         if(xYon < 0 && yYon < 0){
            int i;
			int j = 0;
			
            for(i = 0; i < xFark; i++){
            	
            	if(i == (adimSayisi * (j+1))){
					posPtr[i].y = (current.y + 1); 
            		j++;
				}else{
					posPtr[i].y = current.y;
				}	
            	
            	posPtr[i].x = (current.x + (i));
            	
            	current.y = posPtr[i].y;
            	printf("%d            x : %d , y : %d\n", i, posPtr[i].x, posPtr[i].y);  
            }
        }else if(xYon > 0 && yYon < 0){
            int i;
			int j = 0;
			
            for(i = 0; i < xFark; i++){
            	
            	if(i == (adimSayisi * (j+1))){
					posPtr[i].y = (current.y + 1); 
            		j++;
				}else{
					posPtr[i].y = current.y;
				}	
            	
            	posPtr[i].x = (current.x - (i));
            	
            	current.y = posPtr[i].y;
            	printf("%d            x : %d , y : %d\n", i, posPtr[i].x, posPtr[i].y);  
            }
        }else if(xYon < 0 && yYon > 0){
            int i;
			int j = 0;
			
            for(i = 0; i < xFark; i++){
            	
            	if(i == (adimSayisi * (j+1))){
					posPtr[i].y = (current.y - 1); 
            		j++;
				}else{
					posPtr[i].y = current.y;
				}	
            	
            	posPtr[i].x = (current.x + (i));
            	
            	current.y = posPtr[i].y;
            	printf("%d            x : %d , y : %d\n", i, posPtr[i].x, posPtr[i].y);  
            }
        }else if(xYon > 0 && yYon > 0){
            int i;
			int j = 0;
			
            for(i = 0; i < xFark; i++){
            	
            	if(i == (adimSayisi * (j+1))){
					posPtr[i].y = (current.y - 1); 
            		j++;
				}else{
					posPtr[i].y = current.y;
				}	
            	
            	posPtr[i].x = (current.x - (i));
            	
            	current.y = posPtr[i].y;
            	printf("%d            x : %d , y : %d\n", i, posPtr[i].x, posPtr[i].y);  
            }
        }
    }else{
        adimSayisi = yFark / xFark;
        //posInt = (yFark-(adimSayisi+2));
        posInt = yFark;
        posPtr = calloc(posInt, sizeof(struct position));
        
        if(xYon < 0 && yYon < 0){
            int i;
			int j = 0;
			
            for(i = 0; i < yFark; i++){
            	
            	if(i == (adimSayisi * (j+1))){
					posPtr[i].x = (current.x+1);
            		j++;
				}else{
					posPtr[i].x = current.x;
				}	
            	posPtr[i].y = (current.y + (i)); 
            	current.x = posPtr[i].x;
            	printf("%d            x : %d , y : %d\n", i, posPtr[i].x, posPtr[i].y);  
            }
        }else if(xYon > 0 && yYon < 0){
            int i;
			int j = 0;
			
            for(i = 0; i < yFark; i++){
            	
            	if(i == (adimSayisi * (j+1))){
					posPtr[i].x = (current.x-1);
            		j++;
				}else{
					posPtr[i].x = current.x;
				}	
            	posPtr[i].y = (current.y + (i)); 
            	current.x = posPtr[i].x;
            	printf("%d            x : %d , y : %d\n", i, posPtr[i].x, posPtr[i].y);  
            }
        }else if(xYon < 0 && yYon > 0){
            int i;
			int j = 0;
			
            for(i = 0; i < yFark; i++){
            	
            	if(i == (adimSayisi * (j+1))){
					posPtr[i].x = (current.x+1);
            		j++;
				}else{
					posPtr[i].x = current.x;
				}	
            	posPtr[i].y = (current.y - (i)); 
            	current.x = posPtr[i].x;
            	printf("%d            x : %d , y : %d\n", i, posPtr[i].x, posPtr[i].y);  
            }
        }else if(xYon > 0 && yYon > 0){
            int i;
			int j = 0;
			
            for(i = 0; i < yFark; i++){
            	
            	if(i == (adimSayisi * (j+1))){
					posPtr[i].x = (current.x-1);
            		j++;
				}else{
					posPtr[i].x = current.x;
				}	
            	posPtr[i].y = (current.y - (i)); 
            	current.x = posPtr[i].x;
            	printf("%d            x : %d , y : %d\n", i, posPtr[i].x, posPtr[i].y);  
            }
        }
        
    }
    int a;
	for(a=0; a<(posInt); a++){
    	printf("\nBende þuan bu var :%d %d",posPtr[a].x, posPtr[a].y);
	}
    //return posPtr;
}

// Debug yaparken ekrandaki hareketi görebiliyorum ama ekran temizlemeli olarak çalýþtýrýnca gitmiyor !!!!!!!!
void goTo(struct position current, struct position target){
	
	roadMap(current, target);
	int pChar = maps[posPtr[0].x][posPtr[0].y];
	int i;
	for(i=0; i<posInt; i++){
		wait();
		maps[posPtr[i].x][posPtr[i].y] = 42;

		if(i!=0){
			if(maps[posPtr[i].x][posPtr[i].y] == 0){
				maps[posPtr[i-1].x][posPtr[i-1].y] = 0;
				maps[posPtr[0].x][posPtr[0].y] = pChar;
				system("cls");
				printMap(maps);	
		}else{
			
			i++;
		}	
	}			
}

}


//EpochTime alýyoruz	--- 1 Ocak 1970 tarihinden fonksiyonu çaðýrdýðýmýz ana kadar geçen zamaný saniye cinsinden döner
int  getTime(){
    time_t nowT;
    nowT = time(NULL);
	return nowT;
}
void saveEpochTime(int time){
	FILE *filePtr;
	filePtr = fopen("EpochTime.bin","wb");	
	fwrite(&time, sizeof(time), 1, filePtr);
	fclose(filePtr);
	
}
int getEpochTime(){
	int time;
	FILE *filePtr;
	if((filePtr = fopen("EpochTime.bin","r")) == NULL){
		printf("Epoch Time couldn't get!");
	}else{
		fread(&time, sizeof(time), 1, filePtr);
		fclose(filePtr);
		return time;
	}
	

}
// Bu fonksiyonlar ile oyunun oynanýþ saati hesaplanarak rakiplerin otomatik geliþim algoritmasý oluþturulabilir.










struct position getPosition(){			// Oyun baþlangýcýnda kullancýdan pozisyon alýyoruz
	printf("Yerleþmek istediðiniz pozisyonu giriniz [0-30][0-100] : ");
		int tempX, tempY;
		scanf("%d%d", &tempX, &tempY);
		if((tempX<0 || tempX > 29) || (tempY<0 || tempY > 100)){
			printf("Lütfen bu aralýkta bir deðer giriniz [0-30][0-100]\n");
			getPosition();
		}else{
			if(maps[tempX][tempY] == 0){
				playerPos.x = tempX;
				playerPos.y = tempY;
			}else{
				printf("Bu koordinatlarda baþka bir oyuncu var\n");
				getPosition();
			}
		}
		return playerPos;
}
struct player createRival(){			// Rakip oluþturan fonksiyon
    
    
    int randX = rand()%30;
    int randY = rand()%100;  

    struct player rival;
    if(maps[randX][randY] == 0){
        rival = createPlayer("Rival ", randX,randY);
        addPlayerToMap(maps, rival);
    }else{
        createRival();
    }
	return rival;
}
void createRivals(int numb){			// Rakip dizisine rakipleri atýyoruz
    
    rivals = (struct player*) calloc(numb, sizeof(struct player));
    int i;
    for(i=0; i<numb; i++){
        rivals[i] = createRival();	 
    }
}





		
void printMainScreen(){				// Açýlýþ menüsü
	printf("\n\n\n\t\t_____Hepsini Fethet\'e Hoþ Geldiniz_____\n\n\n");
	
	do{
	    printf("Rakip Sayýsý : ");
	    scanf("%d", &rivalNum);
	    if(rivalNum>11 || rivalNum<1){
	        printf("\nLütfen 1-10 arasýnda bir deðer girin.");
	    }
	}while(rivalNum>11 || rivalNum<1);
	createRivals(rivalNum);	
	printf("Lütfen Adýnýzý Girin : ");
	scanf("%s",playerName);	
	struct position rPos = getPosition();
	realPlayer = createPlayer(playerName, rPos.x, rPos.y);
	realPlayer.id = 0;
	addPlayerToMap(maps, realPlayer);
}
void printPlayerMenu(){				// Oyun içi Menü

	printf("\n\t\t______________________________________________________________");
	printf("\n\t\t_____________________________Menü_____________________________\n");
	printf("\n\t\t1-Askerleri Eðit\t\t\t5-Þehri Taþý");
	printf("\n\t\t2-Kaynaklara Sefer Düzenle\t\t6-Oyuncu Bilgilerini Yaz");
	printf("\n\t\t3-Þehre Saldýr\t\t\t\t7-Kaydet ve Kapat");
	printf("\n\t\t4-Gözcü Gönder");
	doSelections(getSelection());
}
int getSelection(){					// Menü seçeneklerini alýyoruz
	printf("\n\nBirini Seçin : ");
	int selection;
	scanf("%d", &selection);
	return selection;
}
void doSelections(int selection){	// Seçileni yapýyoruz
	int x,y;
	switch(selection){
		case 1:
			trainTroops(&realPlayer);
			refresh();
			break;
		case 2:
			printf("Gidilecek kaynaðýn konumunu giriniz :");
			scanf("%d", &x);
			scanf("%d", &y);
			goToResource(x, y);
			refresh();
			break;
		case 3:
		    warMenu();
		    refresh();
		    break;
		case 4:
		    scoutRecursion();
		    refresh();
		    break;
		case 5:
		    moveToRecursion();
		    //refresh();
		    break;
		case 6:
		    printPlayers();
		    refresh();
		    break;	
		case 7:
		    
		    //Dosyaya kayýt fonksiyonu yazýlýp buradan çaðýrýlacak
		    break;		
	}
		
	
}




void printPlayers(){		// Oyuncularý Ekrana yazdýrýyoruz
	
	printPlayer(realPlayer);
	int i;
	for(i=0; i<rivalNum; i++ ){
		printPlayer(rivals[i]);
	}
}










void refresh(){						// Ekraný yeniliyoruz
    printMessage("\nHarita Tekrar Yükleniyor.");
    printMessage("");
    system("cls");
    printMap(maps);
    printPlayerMenu();
}

void goToResource(int x, int y){	//Kaynaða Gidiyoruz
	
	if((maps[x][y]!=0)){	
		switch(maps[x][y]){
			case 102:
				printMessage("\nHedefe Doðru Yola Çýkýldý.");
				printMessage("\nKaynak Üretimi Baþladý.");
				printMessage("\nÞehre Dönülüyor.");
				realPlayer.playerResource.food += 10;
				printf("\nOyuncunun Güncel Verileri");
				printPlayer(realPlayer);
				break;
			case 119:
				printMessage("\nHedefe Doðru Yola Çýkýldý.");
				printMessage("\nKaynak Üretimi Baþladý.");
				printMessage("\nÞehre Dönülüyor.");
				realPlayer.playerResource.wood += 10;
				printf("\nOyuncunun Güncel Verileri");
				printPlayer(realPlayer);
				break;
			case 115:
				printMessage("\nHedefe Doðru Yola Çýkýldý.");
				printMessage("\nKaynak Üretimi Baþladý.");
				printMessage("\nÞehre Dönülüyor.");
				realPlayer.playerResource.stone += 10;
				printf("\nOyuncunun Güncel Verileri");
				printPlayer(realPlayer);
				break;
			case 103:
				printMessage("\nHedefe Doðru Yola Çýkýldý.");
				printMessage("\nKaynak Üretimi Baþladý.");
				printMessage("\nÞehre Dönülüyor.");
				realPlayer.playerResource.gold += 10;
				printf("\nOyuncunun Güncel Verileri");
				printPlayer(realPlayer);
				break;
			case 105:
				printMessage("\nHedefe Doðru Yola Çýkýldý.");
				printMessage("\nKaynak Üretimi Baþladý.");
				printMessage("\nÞehre Dönülüyor.");
				realPlayer.playerResource.iron += 10;
				printf("\nOyuncunun Güncel Verileri");
				printPlayer(realPlayer);
				break;
			default:
				printf("Bir Sorun Var\nKonum Doðru Belirtilmemiþ Olabilir.");
				goToResourceRecursion();
				break;
		}
	}else{
		printf("Belirttiðiniz Konumda Kaynak Yok\nLütfen Farklý Bir Konum Girin.");
		goToResourceRecursion();
	}	
}
void goToResourceRecursion(){		//Girilen input degerleri uygun degilse tekrar çaðýrmak için recursion yaptýk

    int x, y;
    printf("\nLütfen Kaynak konumu giriniz X Y:");
    scanf("%d%d", &x, &y);
    goToResource(x,y);
}
void moveTo(int x, int y){ 			//Taþýnma fonksiyonu
    
	if(x<0 || x>=30 || y<0 || y>99){
		printf("\nHarita Sýnýrlarý Dýþýna Çýktýnýz. Bilinmeyen Bölgeler Tehlikelidir.");
        moveToRecursion();
	}else if(maps[x][y]==0){
        struct position temp = realPlayer.playerPosition;
        realPlayer.playerPosition.x = x;
        realPlayer.playerPosition.y = y;
        printMessage("Göç Yolculuðu Baþladý.");
        printMessage("\nYeni Konuma Varýldý\nÞehir Yeniden Ýnþa Ediliyor.");
        addPlayerToMap(maps, realPlayer);
        maps[temp.x][temp.y] = 0;
        refresh();
        printf("\nYeni Yurduna Hoþ geldin");
    	printPlayerMenu();
    	
    }else{
        printf("\nÞehri Taþýmaya Çalýþtýðýnýz Konum Dolu");
        moveToRecursion();
    }
}
void moveToRecursion(){				//Girilen input degerleri uygun degilse tekrar çaðýrmak için recursion yaptýk
    int x, y;
    printf("\nLütfen Taþýnmak istediðini konumu giriniz X Y:");
    scanf("%d%d", &x, &y);
    moveTo(x,y);
}
void sendScout(int x, int y){		//Gözcü Gonderiyoruz
	    
    struct player temp;
    
    if(x<0 || x>=30 || y<0 || y>99){
		printf("\nHarita Sýnýrlarý Dýþýna Çýktýnýz. Bilinmeyen Bölgeler Tehlikelidir.");
        scoutRecursion();
	}else{
		if(maps[x][y]>64 && maps[x][y]<75){
        int i;
        for(i=0; i<10; i++){
            if(rivals[i].playerPosition.x == x && rivals[i].playerPosition.y == y){
                temp = rivals[i];
                break;
            }
        }
        int *powers = (int*) malloc(4 * sizeof(int));
	int j;
	for(j =0; j<4; j++){
		powers[j] = getPowers(temp)[j];
	}

	printMessage("Casus, keþif için þehre doðru hareket ediyor.");
	printMessage("\nCasus, þehirden dönüþ yolunda");
	printf("\n\nÞehre gizlice sýzdým ve senin için aþaðýdaki bilgilere ulaþtým\n");
	printf("\nOyuncunun toplam askeri gücü : %d", getTotalPower(temp));
	printf("\n\nAskeri birimlere ait detaylar\nPiyadeler  : %d \nSüvariler	: %d \nOkçular	: %d \nKuþatma Birimleri	: %d ", *powers, *(powers+1),  *(powers+2),  *(powers+3));
	}else{
		printf("\nBu konumda Düþman Yok");
        scoutRecursion();
	}
        
    }
	
	
	
	
	
	
}
void scoutRecursion(){				//Girilen konumun degerleri uygun degilse tekrar çaðýrmak için recursion yaptýk
    int x, y;
    printf("\nLütfen Casus göndermek istediðiniz konumu giriniz X Y:");
    scanf("%d%d", &x, &y);
    sendScout(x,y);
}
















int main(){  

	setlocale(LC_ALL, "Turkish");
	createMap(maps);
	printMainScreen();
	system("cls");
	addResources(maps);
	printMap(maps);
 	printPlayerMenu();

    return 0;
    
}















