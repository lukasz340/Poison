#include <iostream>
#include<time.h>
using namespace std;

#define MAX_KOLOR 7
#define DUZA_LICZBA 20000
#define GRACZ_LOSOWY 0
#define GRACZ_MINIMALIZAJCA 1
#define GRACZ_DAZACY_DO_EKSPLOZJI 2
#define GRACZ_OPTYMALNA_KARTA 3
#define GRACZ_MAKSYMALIZACJA_KOLORU 4

struct Karta {
	int wartosc = 0;
	int kolor = 0;       // green=1  blue=2 red=3 violet=4 yellow=5 white=6 black=7
};
Karta* potasujKarty(Karta*, int);
int liczKartyGracza(Karta*);
int* PodajWartosciNiezielonychKart(int, int*);
Karta** rozdajKarty(Karta**, Karta*, int, int);
void wypiszKolor(int);
void wyswietlSytuacje(int, int, int, Karta*, Karta**, Karta**, Karta**, int, int);
int wybierzStos(Karta, Karta**, int, int);
int wybierzStosGraczMin(Karta, Karta**, int, int);
void zdejmijKarteZReki(Karta*);
void zdejmijKarteZRekiGraczMin(Karta*, int);
bool czyEksplozja(Karta*, int);
void sprawdzOdpornosc(Karta**);
void obliczWynikGracza(Karta**, int, int, bool**);
int wybierzStosGraczMax(Karta, Karta**, int, int);
void ruchDlaGracza2(int, int, int, Karta*, Karta**, Karta**, Karta**, int, int);
void wykonajRuchMaksymalizacjaKolorow(int, int, int, Karta*, Karta**, Karta**, Karta**, int, int, bool**, int);
void wykonajRuchOptymalnaKarta(int, int, int, Karta*, Karta**, Karta**, Karta**, int, int, bool**, int);
void wykonajRuch(int, int, int, Karta*, Karta**, Karta**, Karta**, int, int, bool**, int, bool);
void sprawdzPoprawnoscLiczbaKart(Karta**, int);
void sprawdzKoloryNaKociolkach(Karta**, int);
int liczPunkty(Karta*);
void sprawdzEksplozje(Karta**, int, int);
void sprawdzPoprawnosc(Karta**, int, Karta**, int, int);
void przeprowadzTesty(int, int);




int main() {
	cout << "Czy chcesz przeprowadzic testy skutecznosci graczy?" << endl;
	cout << endl;
	cout << "Wpisz T jezeli chcesz, a N, jezeli nie chcesz" << endl;
	char czy_testy;
	cin >> czy_testy;
	if (czy_testy == 'T') {
		int gracz1;
		int gracz2;
		cout << "Wybierz miedzy ktorymi graczami przeprowadzone zostana testy (od 0 do 4)" << endl;
		cout << "gracz 1:" << endl;
		cin >> gracz1;
		cout << "gracz 2:" << endl;
		cin >> gracz2;
		przeprowadzTesty(gracz1, gracz2);
	}
	cout << "Podaj liczbe rund:" << endl;
	int ile_rund;
	cin >> ile_rund;
	for (int ktora_runda = 0; ktora_runda < ile_rund; ktora_runda++) {
		int liczba_graczy, liczba_kociolkow, liczba_zielonych_kart, wartosc_zielonych_kart, liczba_niezielonych_kart, eksplozja;
		cout << "Podaj kolejno liczbe graczy,liczbe kociolkow, liczbe zielonych kart, wartosc zielonych kart, liczbe niezielonych kart" << endl;
		cin >> liczba_graczy >> liczba_kociolkow >> liczba_zielonych_kart >> wartosc_zielonych_kart >> liczba_niezielonych_kart;

		while (liczba_graczy >= liczba_niezielonych_kart + liczba_zielonych_kart || liczba_graczy < 2) {
			cout << "Podaj liczbe graczy w zakresie od 2 do " << liczba_niezielonych_kart + liczba_zielonych_kart << endl;
			cin >> liczba_graczy;
		}
		cout << "Podaj wartosc eksplozji kociolka:" << endl;
		cin >> eksplozja;

		int* wartosci_niezielonych_kart = new int[liczba_niezielonych_kart];
		PodajWartosciNiezielonychKart(liczba_niezielonych_kart, wartosci_niezielonych_kart);
		int liczba_wszystkich_kart = liczba_zielonych_kart + liczba_niezielonych_kart * liczba_kociolkow;
		Karta* wszystkie_karty = new Karta[liczba_wszystkich_kart];

		for (int numer_zielonej_karty = 0; numer_zielonej_karty < liczba_zielonych_kart; numer_zielonej_karty++) {
			wszystkie_karty[numer_zielonej_karty].kolor = 1;
			wszystkie_karty[numer_zielonej_karty].wartosc = wartosc_zielonych_kart;
		}
		for (int ktory_kolor = 0; ktory_kolor < liczba_kociolkow; ktory_kolor++) {
			for (int ktora_karta = 0; ktora_karta < liczba_niezielonych_kart; ktora_karta++) {
				wszystkie_karty[liczba_zielonych_kart + liczba_niezielonych_kart * ktory_kolor + ktora_karta].wartosc = wartosci_niezielonych_kart[ktora_karta];
				wszystkie_karty[liczba_zielonych_kart + liczba_niezielonych_kart * ktory_kolor + ktora_karta].kolor = ktory_kolor + 2;

			}
		}

		wszystkie_karty = potasujKarty(wszystkie_karty, liczba_wszystkich_kart);
		Karta** reka_gracza = new Karta *[liczba_graczy];
		Karta** zebrane_kerty = new Karta *[liczba_graczy];
		Karta** karty_w_kociolku = new Karta *[liczba_kociolkow];
		bool** tablica_odpornosci = new bool*[liczba_graczy];

		for (int numer_gracza = 0; numer_gracza < liczba_graczy; numer_gracza++) {
			reka_gracza[numer_gracza] = new Karta[liczba_wszystkich_kart / liczba_graczy + 1];
			zebrane_kerty[numer_gracza] = new Karta[liczba_wszystkich_kart];
			tablica_odpornosci[numer_gracza] = new bool[liczba_kociolkow];
			for (int i = 0; i < liczba_kociolkow; i++)
				tablica_odpornosci[numer_gracza][i] = false;
		}

		for (int numer_kociolka = 0; numer_kociolka < liczba_kociolkow; numer_kociolka++) {
			karty_w_kociolku[numer_kociolka] = new Karta[liczba_wszystkich_kart];
		}
		reka_gracza = rozdajKarty(reka_gracza, wszystkie_karty, liczba_wszystkich_kart, liczba_graczy);

		int czyj_ruch = 0;
		bool czy_koniec = false;
		int rodzaj_gracza = 0;
		while (true) {
			wyswietlSytuacje(czyj_ruch, liczba_niezielonych_kart * liczba_kociolkow + liczba_zielonych_kart, liczba_graczy, wszystkie_karty, reka_gracza, zebrane_kerty, karty_w_kociolku, liczba_kociolkow, eksplozja);
			sprawdzPoprawnosc(reka_gracza, liczba_graczy, karty_w_kociolku, liczba_kociolkow, eksplozja);
			if (reka_gracza[czyj_ruch][0].kolor <= 0 || reka_gracza[czyj_ruch][0].kolor > MAX_KOLOR)
				czy_koniec = true;
			if (czyj_ruch == 0)
				wykonajRuch(czyj_ruch, liczba_kociolkow * liczba_niezielonych_kart + liczba_zielonych_kart, liczba_graczy, wszystkie_karty, reka_gracza, zebrane_kerty, karty_w_kociolku, liczba_kociolkow, eksplozja, tablica_odpornosci, 4, false);
			else
				wykonajRuch(czyj_ruch, liczba_kociolkow * liczba_niezielonych_kart + liczba_zielonych_kart, liczba_graczy, wszystkie_karty, reka_gracza, zebrane_kerty, karty_w_kociolku, liczba_kociolkow, eksplozja, tablica_odpornosci, 4, false);
			if (czy_koniec == true)
				break;
			++czyj_ruch = czyj_ruch % liczba_graczy;
		}
		for (int i = 0; i < liczba_graczy; i++) {
			delete[]reka_gracza[i];
			delete[]zebrane_kerty[i];
			delete[]tablica_odpornosci[i];
		}
		for (int i = 0; i < liczba_kociolkow; i++)
			delete[]karty_w_kociolku[i];

		delete[]wartosci_niezielonych_kart;
		delete[]reka_gracza;
		delete[]zebrane_kerty;
		delete[]tablica_odpornosci;
		delete[]wszystkie_karty;
		delete[]karty_w_kociolku;

	}
	system("pause");
	return 0;
}

int* PodajWartosciNiezielonychKart(int liczba_niezielonych_kart, int* wartosci_niezielonych_kart) {
	cout << "podaj wartosci niezielonych kart:" << endl;
	for (int ktora_karta = 0; ktora_karta < liczba_niezielonych_kart; ktora_karta++)
		cin >> wartosci_niezielonych_kart[ktora_karta];
	return wartosci_niezielonych_kart;
}

Karta* potasujKarty(Karta* wszystkie_karty, int liczba_wszystkich_kart) {
	srand(time(NULL));

	for (int i = 0; i < liczba_wszystkich_kart; i++)
	{
		int tasuj = i + (rand() % (liczba_wszystkich_kart - i));
		swap(wszystkie_karty[i], wszystkie_karty[tasuj]);
	}
	return wszystkie_karty;
}

Karta** rozdajKarty(Karta** reka_gracza, Karta* wszystkie_karty, int liczba_kart, int liczba_graczy) {
	for (int i = 0; i < liczba_kart; i++) {
		reka_gracza[i % liczba_graczy][i / liczba_graczy] = wszystkie_karty[i];
	}
	return reka_gracza;
}

void przeprowadzTesty(int gracz1, int gracz2) {
	bool czy_testy = true;
	srand(time(NULL));
	int liczba_gier;
	cout << "Ile gier rozegrac?" << endl;
	cin >> liczba_gier;
	int liczba_graczy = 2;
	int liczba_kociolkow;
	int liczba_zielonych_kart;
	int wartosci_zielonych_kart;
	int liczba_niezielonych_kart;
	int eksplozja;
	int wyniki[3] = { 0,0,0 };

	for (int i = 0; i < liczba_gier; i++) {
		liczba_kociolkow = (2 + rand() % 5);
		liczba_zielonych_kart = (8 + rand() % 20);
		wartosci_zielonych_kart = (2 + rand() % 15);
		liczba_niezielonych_kart = (4 + rand() % 20);
		eksplozja = ((liczba_zielonych_kart + liczba_niezielonych_kart * liczba_kociolkow) / 3);
		int* niezielone = new int[liczba_niezielonych_kart];
		for (int j = 0; j < liczba_niezielonych_kart; j++)
			niezielone[j] = (1 + rand() % 10);
		Karta* wszystkie_karty = new Karta[liczba_kociolkow * liczba_niezielonych_kart + liczba_zielonych_kart];

		for (int numer_zielonej_karty = 0; numer_zielonej_karty < liczba_zielonych_kart; numer_zielonej_karty++) {
			wszystkie_karty[numer_zielonej_karty].kolor = 1;
			wszystkie_karty[numer_zielonej_karty].wartosc = wartosci_zielonych_kart;
		}
		for (int ktory_kolor = 0; ktory_kolor < liczba_kociolkow; ktory_kolor++) {
			for (int ktora_karta = 0; ktora_karta < liczba_niezielonych_kart; ktora_karta++) {
				wszystkie_karty[liczba_zielonych_kart + liczba_niezielonych_kart * ktory_kolor + ktora_karta].kolor = ktory_kolor + 2;
				wszystkie_karty[liczba_zielonych_kart + liczba_niezielonych_kart * ktory_kolor + ktora_karta].wartosc = niezielone[ktora_karta];
			}
		}
		potasujKarty(wszystkie_karty, liczba_niezielonych_kart * liczba_kociolkow + liczba_zielonych_kart);
		Karta** reka_gracza = new Karta *[liczba_graczy];
		Karta** zebrane_kerty = new Karta *[liczba_graczy];
		Karta** karty_w_kociolku = new Karta *[liczba_kociolkow];
		bool** tablica_odpornosci = new bool*[liczba_graczy];

		for (int numer_gracza = 0; numer_gracza < liczba_graczy; numer_gracza++) {
			reka_gracza[numer_gracza] = new Karta[(liczba_kociolkow * liczba_niezielonych_kart + liczba_zielonych_kart) / liczba_graczy + 1];
			zebrane_kerty[numer_gracza] = new Karta[liczba_kociolkow * liczba_niezielonych_kart + liczba_zielonych_kart];
			tablica_odpornosci[numer_gracza] = new bool[liczba_kociolkow];
			for (int i = 0; i < liczba_kociolkow; i++)
				tablica_odpornosci[numer_gracza][i] = false;
		}
		for (int numer_kociolka = 0; numer_kociolka < liczba_kociolkow; numer_kociolka++) {
			karty_w_kociolku[numer_kociolka] = new Karta[liczba_kociolkow * liczba_niezielonych_kart + liczba_zielonych_kart];
		}
		reka_gracza = rozdajKarty(reka_gracza, wszystkie_karty, liczba_kociolkow * liczba_niezielonych_kart + liczba_zielonych_kart, liczba_graczy);

		int czyj_ruch = 0;
		bool czy_koniec = false;
		while (true) {
			//      wyswietlSytuacje(czyj_ruch, liczba_niezielonych_kart * liczba_kociolkow + liczba_zielonych_kart, liczba_graczy, wszystkie_karty, reka_gracza, zebrane_kerty, karty_w_kociolku, liczba_kociolkow, eksplozja);
			sprawdzPoprawnosc(reka_gracza, liczba_graczy, karty_w_kociolku, liczba_kociolkow, eksplozja);
			if (reka_gracza[czyj_ruch][0].kolor <= 0 || reka_gracza[czyj_ruch][0].kolor > MAX_KOLOR)
				czy_koniec = true;
			if (czyj_ruch == 0)
				wykonajRuch(czyj_ruch, liczba_kociolkow * liczba_niezielonych_kart + liczba_zielonych_kart, liczba_graczy, wszystkie_karty, reka_gracza, zebrane_kerty, karty_w_kociolku, liczba_kociolkow, eksplozja, tablica_odpornosci, gracz1, czy_testy);
			else
				wykonajRuch(czyj_ruch, liczba_kociolkow * liczba_niezielonych_kart + liczba_zielonych_kart, liczba_graczy, wszystkie_karty, reka_gracza, zebrane_kerty, karty_w_kociolku, liczba_kociolkow, eksplozja, tablica_odpornosci, gracz2, czy_testy);
			if (czy_koniec == true)
				break;
			++czyj_ruch = czyj_ruch % liczba_graczy;

		}
		int punktacja[2];
		for (int i = 0; i < liczba_graczy; i++) {
			int licznik = 0;
			for (int j = 0; j < liczKartyGracza(zebrane_kerty[i]); j++) {
				if (zebrane_kerty[i][j].kolor == 1) {
					licznik += 2;
					continue;
				}
				if (tablica_odpornosci[i][zebrane_kerty[i][j].kolor - 2] == false)
					licznik++;

			}
			punktacja[i] = licznik;
		}
		if (punktacja[0] > punktacja[1]) {
			wyniki[2]++;
			//              cout << "wygral gracz 2" << endl;
		}

		else if (punktacja[0] < punktacja[1]) {
			wyniki[0]++;
			//      cout << "wygral gracz 1" << endl;
		}
		else {
			//              cout << "remis" << endl;
			wyniki[1]++;
		}
		for (int i = 0; i < liczba_graczy; i++) {
			delete[]reka_gracza[i];
			delete[]zebrane_kerty[i];
			delete[]tablica_odpornosci[i];
		}
		for (int i = 0; i < liczba_kociolkow; i++)
			delete[]karty_w_kociolku[i];

		delete[]niezielone;
		delete[]reka_gracza;
		delete[]zebrane_kerty;
		delete[]karty_w_kociolku;
	}
	cout << "WYNIKI" << endl;
	cout << "gracz " << gracz1 << " wygral " << wyniki[0] << " razy" << endl;
	cout << wyniki[1] << " remisow" << endl;
	cout << "gracz " << gracz2 << " wygral " << wyniki[2] << " razy" << endl;
}


void wypiszKolor(int kolor) {
	switch (kolor) {
	case 2:
		cout << "blue ";
		break;
	case 3:
		cout << "red ";
		break;
	case 4:
		cout << "violet ";
		break;
	case 5:
		cout << "yellow ";
		break;
	case 6:
		cout << "white ";
		break;
	case 7:
		cout << "black ";
		break;
	case 1:
		cout << "green ";
	default:
		break;
	}
}

void wyswietlSytuacje(int czyj_ruch, int liczba_wszystkich_kart, int liczba_graczy, Karta* wszystkie_karty, Karta** reka_gracza, Karta** zebrane_karty, Karta** karty_w_kociolku, int liczba_kociolkow, int eksplozja) {
	cout << "active player: " << czyj_ruch + 1 << endl;
	cout << "players number: " << liczba_graczy << endl;
	cout << "explosion threshold = " << eksplozja << endl;

	for (int i = 0; i < liczba_graczy; i++) {
		cout << "player " << i + 1 << " cards on hand: ";
		for (int j = 0; j < liczKartyGracza(reka_gracza[i]); j++) {
			if (reka_gracza[i][j].kolor <= 0 || reka_gracza[i][j].kolor > MAX_KOLOR)
				break;
			cout << reka_gracza[i][j].wartosc << " ";
			wypiszKolor(reka_gracza[i][j].kolor);
			cout << " ";
		}
		cout << endl;
		cout << "player " << i + 1 << " deck cards: ";
		for (int j = 0; j < liczba_wszystkich_kart; j++) {
			if (zebrane_karty[i][j].kolor <= 0 || zebrane_karty[i][j].kolor > MAX_KOLOR)
				break;
			cout << zebrane_karty[i][j].wartosc << " ";
			wypiszKolor(zebrane_karty[i][j].kolor);
			cout << " ";
		}
		cout << endl;
	}
	for (int i = 0; i < liczba_kociolkow; i++) {
		cout << "pile " << i + 1 << " cards = ";
		for (int j = 0; j < liczba_wszystkich_kart; j++) {
			if (karty_w_kociolku[i][j].kolor <= 0 || karty_w_kociolku[i][j].kolor > MAX_KOLOR)
				break;
			cout << karty_w_kociolku[i][j].wartosc << " ";
			wypiszKolor(karty_w_kociolku[i][j].kolor);
			cout << " ";
		}
		cout << endl;
	}

}
int wybierzStos(Karta wykona_ruch, Karta** karty_w_kociolku, int liczba_kociolkow, int eksplozja) {
	bool* czy_zajete = new bool[liczba_kociolkow];
	for (int i = 0; i < liczba_kociolkow; i++)
		czy_zajete[i] = false;
	if (wykona_ruch.kolor == 1) {
		return 0;

	}
	else {
		for (int i = 0; i < liczba_kociolkow; i++) {
			for (int j = 0; j < liczKartyGracza(karty_w_kociolku[i]); j++) {
				if (karty_w_kociolku[i][j].kolor <= 0 || karty_w_kociolku[i][j].kolor > MAX_KOLOR)
					break;
				if (karty_w_kociolku[i][j].kolor == wykona_ruch.kolor)
					return i;
				if (karty_w_kociolku[i][j].kolor != 1)
					czy_zajete[i] = true;
			}
		}

	}
	for (int i = 0; i < liczba_kociolkow; i++) {
		if (czy_zajete[i] == false) {
			delete[]czy_zajete;
			return i;
		}
	}
	delete[]czy_zajete;
	return 0;
}


int wybierzStosGraczMin(Karta wykona_ruch, Karta** karty_w_kociolku, int liczba_kociolkow, int eksplozja) {
	bool* czy_zajete = new bool[liczba_kociolkow];
	for (int i = 0; i < liczba_kociolkow; i++)
		czy_zajete[i] = false;
	bool czy_zielona = false;
	if (wykona_ruch.kolor == 1) {
		czy_zielona = true;

	}

	int* liczba_punktow_w_kociolkach = new int[liczba_kociolkow];

	for (int i = 0; i < liczba_kociolkow; i++) {
		liczba_punktow_w_kociolkach[i] = 0;
		for (int j = 0; j < liczKartyGracza(karty_w_kociolku[i]); j++) {
			if (karty_w_kociolku[i][j].kolor <= 0 || karty_w_kociolku[i][j].kolor > MAX_KOLOR)
				break;
			liczba_punktow_w_kociolkach[i] += karty_w_kociolku[i][j].wartosc;
			if (karty_w_kociolku[i][j].kolor == wykona_ruch.kolor && wykona_ruch.kolor != 1)
				return i;
			if (karty_w_kociolku[i][j].kolor != 1 && karty_w_kociolku[i][j].kolor != wykona_ruch.kolor)
				czy_zajete[i] = true;
		}
	}

	int minimum = DUZA_LICZBA;
	int ktory = 0;
	for (int i = 0; i < liczba_kociolkow; i++) {
		if (wykona_ruch.kolor == 1) {
			if (liczba_punktow_w_kociolkach[i] < minimum) {
				minimum = liczba_punktow_w_kociolkach[i];
				ktory = i;
			}
		}
		else {
			if (czy_zajete[i] == false) {
				if (liczba_punktow_w_kociolkach[i] < minimum) {
					minimum = liczba_punktow_w_kociolkach[i];
					ktory = i;
				}
			}
		}
	}
	delete[]czy_zajete;
	delete[]liczba_punktow_w_kociolkach;
	return ktory;
}

void zdejmijKarteZReki(Karta* reka_gracza) {
	int i = 0;
	while (true) {
		reka_gracza[i] = reka_gracza[i + 1];
		if (reka_gracza[i].kolor <= 0 || reka_gracza[i].kolor > MAX_KOLOR)
			break;
		i++;
	}
}

void zdejmijKarteZRekiGraczMin(Karta* reka_gracza, int ktora_karta) {
	int i = ktora_karta;
	while (true) {
		reka_gracza[i] = reka_gracza[i + 1];
		if (reka_gracza[i].kolor <= 0 || reka_gracza[i].kolor > MAX_KOLOR)
			break;
		i++;
	}
}

bool czyEksplozja(Karta* karty_na_stosie, int eksplozja) {
	int licznik_punktow = 0;

	for (int i = 0; i < liczKartyGracza(karty_na_stosie); i++) {
		licznik_punktow += karty_na_stosie[i].wartosc;
	}
	if (licznik_punktow > eksplozja) {
		return true;
	}
	else return false;
}

void sprawdzOdpornosc(Karta** karty_na_stosie, int liczba_graczy, int liczba_kociolkow, bool** tablica_odpornosci) {
	for (int k = 0; k < liczba_kociolkow; k++) {
		int max = 0;
		bool czy_aktywne = true;
		int licznik = 0;
		int ktory_gracz = 1;
		for (int i = 0; i < liczba_graczy; i++) {
			licznik = 0;
			for (int j = 0; j < liczKartyGracza(karty_na_stosie[i]); j++) {
				if (karty_na_stosie[i][j].kolor == k + 2)
					licznik++;
			}
			if (licznik > max) {
				max = licznik;
				czy_aktywne = true;
				ktory_gracz = i + 1;
			}
			else if (licznik == max)
				czy_aktywne = false;
		}

		if (czy_aktywne == true) {
			cout << "Na kolor ";
			wypiszKolor(k + 2);
			cout << "jest odporny gracz " << ktory_gracz << endl;
			tablica_odpornosci[ktory_gracz - 1][k] = true;
		}
	}
}

void obliczWynikGracza(Karta** zebrane_karty, int liczba_graczy, int liczba_kociolkow, bool** tablica_odpornosci) {
	for (int i = 0; i < liczba_graczy; i++) {
		int licznik = 0;
		for (int j = 0; j < liczKartyGracza(zebrane_karty[i]); j++) {
			if (zebrane_karty[i][j].kolor == 1) {
				licznik += 2;
				continue;
			}
			if (tablica_odpornosci[i][zebrane_karty[i][j].kolor - 2] == false)
				licznik++;
		}
		cout << "Wynik gracza " << i + 1 << " = " << licznik << endl;
	}
}

int wybierzStosGraczMax(Karta karta_do_ruchu, Karta** karty_w_kociolku, int liczba_kociolkow, int eksplozja) {
	bool* czy_zajete = new bool[liczba_kociolkow];
	for (int i = 0; i < liczba_kociolkow; i++)
		czy_zajete[i] = false;
	bool czy_zielona = false;
	if (karta_do_ruchu.kolor == 1) {
		czy_zielona = true;
	}
	int min = DUZA_LICZBA;
	int licznik_kart = 0;
	int ktory_kociolek = 0;

	for (int i = 0; i < liczba_kociolkow; i++) {
		if (czy_zajete[i] == false) {
			for (int j = 0; j < liczKartyGracza(karty_w_kociolku[i]); j++) {
				licznik_kart++;
				if (karty_w_kociolku[i][j].kolor != 1 && karty_w_kociolku[i][j].kolor != karta_do_ruchu.kolor)
					czy_zajete[i] = true;
				if (karty_w_kociolku[i][j].kolor == karta_do_ruchu.kolor) {
					delete[] czy_zajete;
					return i;
				}
			}
		}
		if (licznik_kart < min && czy_zajete[i] == false) {
			min = licznik_kart;
			ktory_kociolek = i;
		}
		licznik_kart = 0;
	}
	delete[]czy_zajete;
	return ktory_kociolek;
}

void ruchDlaGracza2(int czyj_ruch, int liczba_wszystkich_kart, int liczba_graczy, Karta* wszystkie_karty, Karta** reka_gracza, Karta** zebrane_karty, Karta** karty_w_kociolku, int liczba_kociolkow, int eksplozja) {
	int maksimum = 0;
	int numer_karty = 0;
	Karta karta_do_ruchu;

	for (int i = 0; i < liczKartyGracza(reka_gracza[czyj_ruch]); i++) {
		if (reka_gracza[czyj_ruch][i].wartosc > maksimum) {
			maksimum = reka_gracza[czyj_ruch][i].wartosc;
			numer_karty = i;
			karta_do_ruchu = reka_gracza[czyj_ruch][i];
		}
	}

	int kociolek_do_ruchu = 0;
	kociolek_do_ruchu = wybierzStosGraczMax(karta_do_ruchu, karty_w_kociolku, liczba_kociolkow, eksplozja);
	zdejmijKarteZRekiGraczMin(reka_gracza[czyj_ruch], numer_karty);
	int i = 0;
	while (true) {
		if (karty_w_kociolku[kociolek_do_ruchu][i].kolor <= 0 || karty_w_kociolku[kociolek_do_ruchu][i].kolor > MAX_KOLOR) {
			karty_w_kociolku[kociolek_do_ruchu][i] = karta_do_ruchu;
			break;
		}
		i++;
	}
	bool czy_wykona_eksplozje = czyEksplozja(karty_w_kociolku[kociolek_do_ruchu], eksplozja);

	if (czy_wykona_eksplozje == true) {
		int j = 0;
		while (true) {
			if (karty_w_kociolku[kociolek_do_ruchu][j].kolor <= 0 || karty_w_kociolku[kociolek_do_ruchu][j].kolor > MAX_KOLOR) {
				break;
			}
			int k = 0;
			while (true) {
				if (zebrane_karty[czyj_ruch][k].kolor <= 0 || zebrane_karty[czyj_ruch][k].kolor > MAX_KOLOR) {
					zebrane_karty[czyj_ruch][k] = karty_w_kociolku[kociolek_do_ruchu][j];
					break;

				}
				k++;
			}
			j++;
		}
		int k = 0;
		while (true) {
			if (karty_w_kociolku[kociolek_do_ruchu][k].kolor <= 0 || karty_w_kociolku[kociolek_do_ruchu][k].kolor > MAX_KOLOR)
				break;
			karty_w_kociolku[kociolek_do_ruchu][k].kolor = 0;
			karty_w_kociolku[kociolek_do_ruchu][k].wartosc = 0;
			k++;
		}
	}
}

void wykonajRuchMaksymalizacjaKolorow(int czyj_ruch, int liczba_wszystkich_kart, int liczba_graczy, Karta* wszystkie_karty, Karta** reka_gracza, Karta** zebrane_karty, Karta** karty_w_kociolku, int liczba_kociolkow, int eksplozja, bool** tablica_odpornosci, int rodzaj_gracza) {
	int* tablica_kolorow = new int[liczba_kociolkow];
	for (int i = 0; i < liczba_kociolkow; i++)
		tablica_kolorow[i] = 0;
	for (int i = 0; i < liczba_kociolkow; i++) {
		for (int j = 0; j < liczKartyGracza(karty_w_kociolku[i]); j++) {
			if (karty_w_kociolku[i][j].kolor != 1)
				tablica_kolorow[i]++;
		}
	}

	int max = 0;
	int index = 0;
	for (int i = 0; i < liczKartyGracza(reka_gracza[czyj_ruch]); i++) {
		int ktory_stos;
		if (reka_gracza[czyj_ruch][i].kolor != 1)
			ktory_stos = wybierzStos(reka_gracza[czyj_ruch][i], karty_w_kociolku, liczba_kociolkow, eksplozja);
		else {
			int stos = 0;
			int najwieksza = 0;
			for (int j = 0; j < liczba_kociolkow; j++) {
				if (tablica_kolorow[j] > najwieksza) {
					najwieksza = tablica_kolorow[j];
					stos = j;
				}
			}
			ktory_stos = stos;
		}
		if (reka_gracza[czyj_ruch][i].kolor == 1) {
			if (tablica_kolorow[ktory_stos] > max) {
				max = tablica_kolorow[ktory_stos];
				index = i;
			}
		}
		else
			if (tablica_kolorow[ktory_stos] + 1 > max) {
				max = tablica_kolorow[ktory_stos] + 1;
				index = i;
			}
	}
	int ktory_stos = wybierzStos(reka_gracza[czyj_ruch][index], karty_w_kociolku, liczba_kociolkow, eksplozja);
	int j = 0;
	while (true) {
		if (karty_w_kociolku[ktory_stos][j].kolor <= 0 || karty_w_kociolku[ktory_stos][j].kolor > MAX_KOLOR) {
			karty_w_kociolku[ktory_stos][j] = reka_gracza[czyj_ruch][index];
			break;
		}
		j++;
	}
	j = 0;
	while (true) {
		if (karty_w_kociolku[ktory_stos][j].kolor <= 0 || karty_w_kociolku[ktory_stos][j].kolor > MAX_KOLOR) {
			break;
		}
		int k = 0;
		while (true) {
			if (zebrane_karty[czyj_ruch][k].kolor <= 0 || zebrane_karty[czyj_ruch][k].kolor > MAX_KOLOR) {
				zebrane_karty[czyj_ruch][k] = karty_w_kociolku[ktory_stos][j];
				break;
			}
			k++;
		}
		j++;
	}
	int k = 0;
	while (true) {
		if (karty_w_kociolku[ktory_stos][k].kolor <= 0 || karty_w_kociolku[ktory_stos][k].kolor > MAX_KOLOR)
			break;
		karty_w_kociolku[ktory_stos][k].kolor = 0;
		karty_w_kociolku[ktory_stos][k].wartosc = 0;
		k++;
	}
	zdejmijKarteZRekiGraczMin(reka_gracza[czyj_ruch], index);
	delete[]tablica_kolorow;
};

void wykonajRuchOptymalnaKarta(int czyj_ruch, int liczba_wszystkich_kart, int liczba_graczy, Karta* wszystkie_karty, Karta** reka_gracza, Karta** zebrane_karty, Karta** karty_w_kociolku, int liczba_kociolkow, int eksplozja, bool** tablica_odpornosci, int rodzaj_gracza) {
	bool* czy_sprawdzone = new bool[liczKartyGracza(reka_gracza[czyj_ruch])];
	for (int i = 0; i < liczKartyGracza(reka_gracza[czyj_ruch]); i++)
		czy_sprawdzone[i] = false;
	while (true) {
		int kociolek_do_ruchu = 0;
		int max = 0;
		int indeks_najwiekszej = 0;
		int zlicz_punkty = 0;
		bool czy_nie_mozna = true;
		for (int i = 0; i < liczKartyGracza(reka_gracza[czyj_ruch]); i++) {
			if (czy_sprawdzone[i] == false) {
				if (reka_gracza[czyj_ruch][i].wartosc > max) {
					max = reka_gracza[czyj_ruch][i].wartosc;
					indeks_najwiekszej = i;
					czy_nie_mozna = false;
				}
			}
		}
		if (czy_nie_mozna == true) {
			if (rodzaj_gracza == GRACZ_OPTYMALNA_KARTA)
				ruchDlaGracza2(czyj_ruch, liczba_wszystkich_kart, liczba_graczy, wszystkie_karty, reka_gracza, zebrane_karty, karty_w_kociolku, liczba_kociolkow, eksplozja);
			else if (rodzaj_gracza == GRACZ_MAKSYMALIZACJA_KOLORU)
				wykonajRuchMaksymalizacjaKolorow(czyj_ruch, liczba_wszystkich_kart, liczba_graczy, wszystkie_karty, reka_gracza, zebrane_karty, karty_w_kociolku, liczba_kociolkow, eksplozja, tablica_odpornosci, rodzaj_gracza);
			delete[]czy_sprawdzone;
			return;
		}
		if (reka_gracza[czyj_ruch][indeks_najwiekszej].kolor != 1)
			kociolek_do_ruchu = wybierzStos(reka_gracza[czyj_ruch][indeks_najwiekszej], karty_w_kociolku, liczba_kociolkow, eksplozja);
		else {
			bool czy_znaleziono = false;
			int najwieksza = 0;
			int liczba_punktow = 0;
			for (int j = 0; j < liczba_kociolkow; j++) {
				liczba_punktow = 0;
				for (int k = 0; k < liczKartyGracza(karty_w_kociolku[j]); k++)
					liczba_punktow += karty_w_kociolku[j][k].wartosc;
				liczba_punktow += reka_gracza[czyj_ruch][indeks_najwiekszej].wartosc;
				if (liczba_punktow <= eksplozja) {
					najwieksza = j;
					czy_znaleziono = true;
				}
				if (czy_znaleziono == true)
					break;
			}
			kociolek_do_ruchu = najwieksza;
		}
		zlicz_punkty += (reka_gracza[czyj_ruch][indeks_najwiekszej].wartosc + liczPunkty(karty_w_kociolku[kociolek_do_ruchu]));
		if (zlicz_punkty <= eksplozja) {
			int j = 0;
			while (true) {
				if (karty_w_kociolku[kociolek_do_ruchu][j].kolor <= 0 || karty_w_kociolku[kociolek_do_ruchu][j].kolor > MAX_KOLOR) {
					karty_w_kociolku[kociolek_do_ruchu][j] = reka_gracza[czyj_ruch][indeks_najwiekszej];
					break;
				}
				j++;
			}
			zdejmijKarteZRekiGraczMin(reka_gracza[czyj_ruch], indeks_najwiekszej);
			delete[]czy_sprawdzone;
			return;
		}
		czy_sprawdzone[indeks_najwiekszej] = true;
	}
	delete[]czy_sprawdzone;
}

void wykonajRuch(int czyj_ruch, int liczba_wszystkich_kart, int liczba_graczy, Karta* wszystkie_karty, Karta** reka_gracza, Karta** zebrane_karty, Karta** karty_w_kociolku, int liczba_kociolkow, int eksplozja, bool** tablica_odpornosci, int rodzaj_gracza, bool czy_testy) {
	if (czy_testy == false) {
		if (liczKartyGracza(reka_gracza[czyj_ruch]) == 0) {
			sprawdzOdpornosc(zebrane_karty, liczba_graczy, liczba_kociolkow, tablica_odpornosci);
			obliczWynikGracza(zebrane_karty, liczba_graczy, liczba_kociolkow, tablica_odpornosci);
		}
	}

	if (rodzaj_gracza == GRACZ_OPTYMALNA_KARTA || rodzaj_gracza == GRACZ_MAKSYMALIZACJA_KOLORU) {
		wykonajRuchOptymalnaKarta(czyj_ruch, liczba_wszystkich_kart, liczba_graczy, wszystkie_karty, reka_gracza, zebrane_karty, karty_w_kociolku, liczba_kociolkow, eksplozja, tablica_odpornosci, rodzaj_gracza);
		return;
	}
	Karta wykona_ruch;
	int kociolek_do_ruchu = 0;
	int ktora_karta = 0;
	if (rodzaj_gracza == GRACZ_LOSOWY)
		wykona_ruch = reka_gracza[czyj_ruch][0];
	else if (rodzaj_gracza == GRACZ_MINIMALIZAJCA || rodzaj_gracza == GRACZ_DAZACY_DO_EKSPLOZJI) {
		ktora_karta = 0;
		wykona_ruch = reka_gracza[czyj_ruch][0];
		int minimalna = reka_gracza[czyj_ruch][0].wartosc;
		for (int i = 0; i < liczKartyGracza(reka_gracza[czyj_ruch]); i++) {
			if (reka_gracza[czyj_ruch][i].wartosc < minimalna) {
				minimalna = reka_gracza[czyj_ruch][i].wartosc;
				wykona_ruch = reka_gracza[czyj_ruch][i];
				ktora_karta = i;
			}
		}
		kociolek_do_ruchu = wybierzStosGraczMin(wykona_ruch, karty_w_kociolku, liczba_kociolkow, eksplozja);
	}
	if (rodzaj_gracza == GRACZ_LOSOWY)
		kociolek_do_ruchu = wybierzStos(wykona_ruch, karty_w_kociolku, liczba_kociolkow, eksplozja);
	if (rodzaj_gracza == GRACZ_MINIMALIZAJCA || rodzaj_gracza == GRACZ_LOSOWY) {
		for (int i = 0; i < liczKartyGracza(karty_w_kociolku[kociolek_do_ruchu]) + 1; i++) {
			if (karty_w_kociolku[kociolek_do_ruchu][i].kolor <= 0 || karty_w_kociolku[kociolek_do_ruchu][i].kolor > MAX_KOLOR) {
				karty_w_kociolku[kociolek_do_ruchu][i] = wykona_ruch;
				break;
			}

		}

	}

	int zlicz_punkty = 0;
	bool czy_wykona_eksplozje = false;
	if (rodzaj_gracza == GRACZ_DAZACY_DO_EKSPLOZJI)
		zlicz_punkty += (wykona_ruch.wartosc + liczPunkty(karty_w_kociolku[kociolek_do_ruchu]));
	if (rodzaj_gracza == GRACZ_LOSOWY)
		zdejmijKarteZReki(reka_gracza[czyj_ruch]);
	else if (rodzaj_gracza == GRACZ_MINIMALIZAJCA) {
		zdejmijKarteZRekiGraczMin(reka_gracza[czyj_ruch], ktora_karta);
	}
	if (rodzaj_gracza == GRACZ_MINIMALIZAJCA || rodzaj_gracza == GRACZ_LOSOWY)
		czy_wykona_eksplozje = czyEksplozja(karty_w_kociolku[kociolek_do_ruchu], eksplozja);

	if (rodzaj_gracza == GRACZ_DAZACY_DO_EKSPLOZJI && zlicz_punkty > eksplozja) {
		ruchDlaGracza2(czyj_ruch, liczba_wszystkich_kart, liczba_graczy, wszystkie_karty, reka_gracza, zebrane_karty, karty_w_kociolku, liczba_kociolkow, eksplozja);
		return;
	}
	if (rodzaj_gracza == GRACZ_DAZACY_DO_EKSPLOZJI && zlicz_punkty <= eksplozja) {
		int i = 0;
		while (true) {
			if (karty_w_kociolku[kociolek_do_ruchu][i].kolor <= 0 || karty_w_kociolku[kociolek_do_ruchu][i].kolor > MAX_KOLOR) {
				karty_w_kociolku[kociolek_do_ruchu][i] = wykona_ruch;
				break;
			}
			i++;
		}
		zdejmijKarteZRekiGraczMin(reka_gracza[czyj_ruch], ktora_karta);
	}
	if (czy_wykona_eksplozje == true) {
		int j = 0;
		while (true) {
			if (karty_w_kociolku[kociolek_do_ruchu][j].kolor <= 0 || karty_w_kociolku[kociolek_do_ruchu][j].kolor > MAX_KOLOR) {
				break;
			}
			int k = 0;
			while (true) {
				if (zebrane_karty[czyj_ruch][k].kolor <= 0 || zebrane_karty[czyj_ruch][k].kolor > MAX_KOLOR) {
					zebrane_karty[czyj_ruch][k] = karty_w_kociolku[kociolek_do_ruchu][j];
					break;
				}
				k++;
			}
			j++;
		}
		int k = 0;
		while (true) {
			if (karty_w_kociolku[kociolek_do_ruchu][k].kolor <= 0 || karty_w_kociolku[kociolek_do_ruchu][k].kolor > MAX_KOLOR)
				break;
			karty_w_kociolku[kociolek_do_ruchu][k].kolor = 0;
			karty_w_kociolku[kociolek_do_ruchu][k].wartosc = 0;
			k++;
		}
	}
}

int liczKartyGracza(Karta* karty) {
	int i = 0;
	while (true) {
		if (karty[i].kolor <= 0 || karty[i].kolor > MAX_KOLOR) {
			break;
		}
		i++;
	}
	return i;
}

void sprawdzPoprawnoscLiczbaKart(Karta** karty, int liczba_graczy) {
	int najwieksza = liczKartyGracza(karty[0]);
	int najmniejsza = najwieksza;
	for (int i = 1; i < liczba_graczy; i++) {
		if (liczKartyGracza(karty[i]) > najwieksza)
			najwieksza = liczKartyGracza(karty[i]);
		if (liczKartyGracza(karty[i]) < najmniejsza)
			najmniejsza = liczKartyGracza(karty[i]);
	}
	if (najwieksza - najmniejsza > 1)
		cout << "The number of players cards on hand is wrong" << endl;
}

void sprawdzKoloryNaKociolkach(Karta** karty_w_kociolkach, int liczba_kociolkow) {
	for (int i = 0; i < liczba_kociolkow; i++) {
		int licznik = 0;
		Karta zmienna;

		for (int j = 0; j < liczKartyGracza(karty_w_kociolkach[i]); j++) {
			if (karty_w_kociolkach[i][j].kolor != 1 && karty_w_kociolkach[i][j].kolor != zmienna.kolor) {
				zmienna = karty_w_kociolkach[i][j];
				licznik++;
				if (licznik > 1) {
					cout << "Two different colors were found on the " << i + 1 << " pile" << endl;
					break;
				}
			}
		}
	}
}
int liczPunkty(Karta* karta) {
	int ile_punktow = 0;
	for (int i = 0; i < liczKartyGracza(karta); i++) {
		ile_punktow += karta[i].wartosc;
	}
	return ile_punktow;
}

void sprawdzEksplozje(Karta** karta, int liczba_kociolkow, int eksplozja) {
	for (int i = 0; i < liczba_kociolkow; i++) {
		if (liczPunkty(karta[i]) > eksplozja)
			cout << "Pile number " << i + 1 << " should explode earlier" << endl;
	}
}

void sprawdzPoprawnosc(Karta** karty, int liczba_graczy, Karta** karty_w_kociolkach, int liczba_kociolkow, int eksplozja) {
	sprawdzPoprawnoscLiczbaKart(karty, liczba_graczy);
	sprawdzKoloryNaKociolkach(karty_w_kociolkach, liczba_kociolkow);
	sprawdzEksplozje(karty_w_kociolkach, liczba_kociolkow, eksplozja);
}