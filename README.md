# Sudoku board BMP
## Sudoku solver dintr-o imagine BMP
### Structura formatului BMP
O imagine BMP are urmatoarea structura:
-   un  **File Header**  care are urmatoarele campuri:
    1.  **signature**  – 2 octeti - literele 'B' si 'M' in  ASCII;
    2.  **file size**  – 4 octeti – dimensiunea intregului fisier;
    3.  **reserved**  – 4 octeti – nefolosit;
    4.  **offset**  – 4 octeti – offsetul de la inceputul fisierului pana la inceputului bitmap-ului, adica al matricei de pixeli.
-   un  **Info Header**  care poate avea structuri diferite, insa noi vom lucra cu cel care se numeste  **BITMAPINFOHEADER**. Are urmatoarele campuri:
    1.  **size**  – 4 octeti – dimensiunea Info Header-ului, care are o valoare fixa, 40;
    2.  **width**  – 4 octeti – latimea matricei de pixeli (numarul de coloane);
    3.  **height**  – 4 octeti – inaltimea matricei de pixeli (numarul de randuri);
    4.  **planes**  – 2 octeti – setat la valoarea fixa 1;
    5.  **bit count**  – 2 octeti – numarul de biti per pixel. In cazul nostru va avea mereu valoarea 24, adica reprezentam fiecare pixel pe 3 octeti, adica cele 3 canale, RGB;
    6.  **compression**  – 4 octeti – tipul de compresie. Acest camp va fi 0;
    7.  **image size**  – 4 octeti – se refera la dimensiunea matricei de pixeli, inclusiv padding-ul adaugat (vedeti mai jos);
    8.  **x pixels per meter**  – 4 octeti – se refera la rezolutia de printare. Pentru a simplifica putin tema, veti seta acest camp pe 0. Nu o sa printam imaginile :);   
    9.  **y pixels per meter**  – la fel ca mai sus;
    10.  **colors used**  – numarul de culori din paleta de culori. Aceasta este o sectiune care va lipsi din imaginile noastre BMP, deoarece ea se afla in general imediat dupa  **Info Header**  insa doar pentru imaginile care au campul  **bit count**  mai mic sau egal cu 8. Prin urmare, campul va fi setat pe 0;
    11.  **colors important**  – numarul de culori importante. Va fi, de asemenea, setat pe 0, ceea ce inseamna ca toate culorile sunt importante.
-   **BitMap**-ul, care este matricea de pixeli si care ocupa cea mai mare parte din fisier. Trei lucruri trebuie mentionate despre aceasta:
    1.  pixelii propriu-zisi se afla intr-o matrice de dimensiune  **height x width**, insa ea poate avea o dimensiune mai mare de atat din cauza  **paddingului**. Acest padding este adaugat la sfarsitul fiecarei linii astfel incat fiecare linie sa inceapa de la o adresa (offset fata de inceputul fisierului) multiplu de 4. Mare atentie la citire, pentru ca acest padding trebuie  **ignorat**  (fseek). De asemenea, la scriere va trebui sa puneti  **explicit**  valoarea 0 pe toti octetii de padding.
    2.  este  **rasturnata**, ceea ce inseamna ca prima linie in matrice contine de fapt pixelii din extremitatea de jos a imaginii. Vedeti exemplul de mai jos;
    3.  canelele pentru fiecare pixel sunt in ordinea BGR (**B**lue  **G**reen  **R**ed).

### Task 1
Primim un board sudoku prin intermediul unui fisier in format JSON. Sarcina este sa transform textul JSON intr-o imagine BMP. Datele de intrare sunt parsate cu ajutorul bibliotecii externe [cJSON](https://github.com/DaveGamble/cJSON).
#### Exemplu input
` board_0.json`
```
{
	"bitmap":	[0,0,0,...,0,0,0,0,0,0,0,255,255,255,255,255,255,...],
	"file_header":	{
		"offset":	54,
		"signature":	"BM",
		"reserved":	0,
		"file_size":	16114
	},
	"info_header":	{
		"colors_used":	0,
		"size":	40,
		"colors_important":	0,
		"image_size":	16060,
		"y_pixels_per_meter":	0,
		"x_pixels_per_meter":	0,
		"width":	73,
		"planes":	1,
		"bit_count":	24,
		"compression":	0,
		"height":	73
	}
}
```
**Output** : o imagine in format bmp formata din informatia din fisierul JSON.

#### Informatii despre imagine
Imaginea este formata dintr-o multime de pixeli, fiecare pixel reprezentat prin 3 valori (R,G,B). 
Imaginea este de 73x73 pixel, 9x9 patratele care respecta urmatoarele dimensiuni:

 1. o margine de 1 pixel neagra/gri
 2. o margine de pixel alba in jurul cifrei
 3. cifra de 5x5

Marginile nu se dubleaza nicaieri. Peste tot in imagine marginile au grosimea de 1 pixel. Marginile care delimiteaza un subpatrat sunt negre, dar cele din interiorul sectiunii sunt gri.

### Task 2
Dupa ce inspectam imaginea formata din datele primite la task 1 observam ca cifrele sunt mirrored. Ce trebuia sa fac aici este sa oglindez imaginea primita.
**Input**: Imaginea de la cerinta anterioara.

**Output**:

![output_task_2](https://i.imgur.com/fhigkLi.png)

### Task 3
Dupa prelucrarile asupra setului de date primit la taskurile anterioare, putem verifica daca board-ul sudoku este completat corect sau nu. Se implica recunoasterea de cifre dintr-o imagine bmp.

**Input**: Imaginea obtinuta ca output la cerinta anterioara.
**Output**: 
`output_task3.json`
```
{
	"input_file":	"board0.json",
	"game_state":	"Win!"
}
```

### Task 4
La acest task voi citi o imagine .bmp cu cifre lipsa si voi completa spatiile goale cu cifrele potrivite. Cifrele scrise vor avea alta culoare fata de cele din imaginea initiala: magenta (255 0 255).
**Input**:

![input_task4](https://i.imgur.com/tuUjVtb.jpg)

**Output**:

![output_task_4](https://i.imgur.com/ZAiDvWo.jpg)

### Task bonus
Aici vom rezolva un joc sudoku de la 0. Voi primi o imagine bmp cu o tabla sudoku necompletata, va trebui sa rezolv jocul folosind un algoritm de rezolvare sudoku. 
Cifrele lipsa vor fi completata cu magenta. In cazul in care jocul nu poate fi completat va trebui sa suprascriem fiecare cifra din imagine cu un X, scirs cu rosu (255 0 0).
**Input**:

![input_task_bonus](https://i.imgur.com/DlhramW.png)

**Output input rezolvabil**:

![output_bonus_rezolvabil](https://i.imgur.com/fK1LPN1.png)

**Output input nerezolvabil**:

![output_bonus_nerezolvabil](https://i.imgur.com/DyqGPEj.png)

### Restrictii si precizari
-   Fisierele de intrare vor fi primite ca argumente in linia de comanda, iar fisierele de iesire pentru fiecare task vor fi denumite in felul urmator “output_<task1/2/3/4/>/bonus_board?.<json/bmp>”, unde ”?” reprezinta numarul boardului primit ca intrare.
-  Tema se va rula in felul urmator: ”./sudoku input <123/4/bonus>”, al doilea parametru este necesar pentru a se putea rula checkerul.
- Se ruleaza pe rand task 1, 2, 3  **sau**  task 4  **sau**  task bonus.
-   Pentru taskurile 1, 2 si 3 datele se citesc o singura data din fisierul .json si se pastreaza in memorie  **optim**  imaginea. In caz contrar puteti suferi depunctari.


### Copyright
Drepturile asupra fisierelor din folderul `checker`,cat si asupra enuntului temei sunt rezervate de catre echipa PC 2020-2021, facultatea AC, UPB. Drepturile asupra restul fisierelor sunt rezervate de catre autorul acestui repo ([vanyadarkov](https://github.com/vanyadarkov)).
