#include <iostream>

using namespace std;

void plansza(char t[])
{
  for(int i = 1; i <= 9; i++)
  {
    cout << " " << t[i] << " ";
    if(i % 3)       cout << "|";
    else if(i != 9) cout << "\n---+---+---\n";
    else            cout << endl;
  }
}

// true - następuje wygrana zawodnika
bool wygrana(char t[], char g, bool cisza)
{
  bool test;
  int i;

  test = false; // true jeśli gracz ma 03 figury w kolumnie, wierszu lub na przekątnej
// Sprawdza wiersze

  for (i = 1; i <= 7; i += 3) {
	  test |= ((t[i] == g) && (t[i + 1] == g) && (t[i + 2] == g));
  }

// Sprawdza kolumny

  for(i = 1; i <= 3; i++)    test |= ((t[i] == g) && (t[i+3] == g) && (t[i+6] == g));

// Sprawdza przek¹tn¹ 1-5-9

  test |= ((t[1] == g) && (t[5] == g) && (t[9] == g));

// Sprawdza przek¹tn¹ 3-5-7

  test |= ((t[3] == g) && (t[5] == g) && (t[7] == g));

  if(test)
  {
    if(!cisza)
    {
      plansza(t);
      cout << "\nGRACZ " << g << " WYGRYWA!!!\n\n";
    }
    return true;
  }
  return false;
}

// true jeśli wszystkie pola są zajęte (wole pole to spacja)

bool remis(char t[], bool cisza)
{

  for(int i = 1; i <= 9; i++) if(t[i] == ' ') return false;

// Jeœli pêtla for zakoñczy³a siê normalnie, to na ¿adnym polu planszy nie by³o
// spacji. Mamy do czynienia z remisem - zwracamy true

  if(!cisza)
  {
    plansza(t); cout << "\nREMIS !!!\n\n";
  }
  return true;
}

// Algorytm rekurencyjny MINIMAX
int minimax(char t[], char gracz)
{
  int m, mmx;
  if(wygrana(t,gracz,true)) return (gracz == 'X') ? 1 : -1;
  if(remis(t,true)) return 0;
  gracz = (gracz == 'X') ? 'O' : 'X';
  mmx = (gracz == 'O') ? 10 : -10;
  for(int i = 1; i <= 9; i++)
    if(t[i] == ' ')
    {
       t[i] = gracz;
       m = minimax(t,gracz);
       t[i] = ' ';
       if(((gracz == 'O') && (m < mmx)) || ((gracz == 'X') && (m > mmx))) mmx = m;
    }
  return mmx;
}

// Funkcja wyznacza ruch dla komputera.
int komputer(char t[])
{
  int ruch, i, m, mmx;

  mmx = -10;
  for(i = 1; i <= 9; i++)
    if(t[i] == ' ')
    {
      t[i] = 'X';
      m = minimax(t,'X');
      t[i] = ' ';
      if(m > mmx)
      {
        mmx = m; ruch = i;
      }
    }
  return ruch;
}

// Funkcja umo¿liwia ruch gracza
// Po ruchu nastêpuje zamiana gracza
//------------------------------------
void ruch(char t[], char &gracz)
{
  int r;

  plansza(t);
  if(gracz == 'O')
  {
    cout << "\nCZLOWIEK : wybiera ruch : ";
    cin >> r;
  }
  else
  {
    r = komputer(t);
    cout << "\nKOMPUTER : wybiera ruch : " << r << endl;
  }
  cout << "---------------------------\n\n";
  if((r >= 1) && (r <= 9) && (t[r] == ' ')) t[r] = gracz;
  gracz = (gracz == 'O') ? 'X' : 'O';
}

void main()
{
  char t[10],gracz,wybor;

  do
  {
    cout << "Gra w Kolko i Krzyzyk dla gracza i komputera\n"
            "============================================\n\n";
    for(int i = 1; i <= 9; i++) t[i] = ' ';
    gracz = 'O';

    while(!wygrana(t,'X',false) && !wygrana(t,'O',false) && !remis(t,false)) ruch(t,gracz);

    cout << "Jeszcze raz ? (T = TAK) : ";
    cin >> wybor;
    cout << "\n\n\n";

  } while((wybor == 'T') || (wybor == 't'));
}
