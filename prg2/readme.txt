*Harjoitustyön 2. osan kommentit seuraavat 1. osan kommenttien yhteyteen
*työn kokonaisuuden loogista esittämistä edistäen.

*Uudet kommentit alkavat asteriskilla.

Tehokkuusmerkintöjä on tehty myös koodin kommentointiin.


Valitut tietorakenteet:

    Ympäristössä on havaittavissa helposti kokonaisuuksia, jotka on mielekästä
    koota omiksi tietorakenteikseen. Näitä ovat paikat (Places), alueet (Areas)
    ja koordinaattien etäisyyslajitteluun soveltuva paikkakoordinaattielementti
    (placeCoordItem), mikä ennakoiden soveltuu hyvin pitämään sisällään
    mahdollisen "tien" tiedot.

    Kuvatut kokonaisuudet eivät kaipaa toiminnallisuuksia, joten tiedot on
    järkevintä koota Struct -tietorakenteeseen.

    Structit:

        struct Place
        {
            Name pName = "";
            PlaceType pType = {};
            Coord pCoord = {};
        };

        Tarpeelliset tiedot paikan määrittelylle; nimi, tyyppi ja piste


        struct Area
        {
            Name aName = "";
            std::vector<Coord> aCoords = {};
            AreaID uberArea = NO_AREA;
            std::vector<AreaID> subArea = {};
        };

        Edellisten lisäksi haluamme, alueiden välisiä hierarkiasuhteita
        vertaillaksemme, myös tietää hierarkiassa suoraan ylemmän
        alueen, joita voi olla vain yksi sekä kaikki alemmat alueet, joita
        voi olla useita.


        placeCoordItem
        {
            PlaceID myPlaceId;
            Coord   myPlaceCoordA;
            Coord   myPlaceCoordB;
            double distance = 0;

            bool operator<(const placeCoordItem& a) const
            {
                if (distance == a.distance)
                {
                    return myPlaceCoordA.y < a.myPlaceCoordB.y;
                }
                return  distance < a.distance;
            }
        };

        Kyseessä on tietorakenne, jolla on paikka, identiteetti, tieto määränpäästä
        ja etäisyys siihen.

        Tehtävässä tulee yhtenä osatehtävänä järjestää koordinaatteja
        etäisyysjärjestyksiin annettujen sääntöjen mukaisesti. Siksi operaattori
        < on korvattu vertailulla distance ja toissijaisesti y. Sortin
        tehokkuus O(nlog n).

* Edellinen rakenne oli tarkoitettu palvelemaan tienkin tietorakenteena, mutta
* koska y -koordinaatin mukainen lajittelu jostain syystä kääntyi väärin päin
* lopputulosteessa, katsoin viisaammaksi toteuttaa koko way/crossroad -graafin
* kokonaan uudelleen alusta, jotta turhilta ja verrattain mittavilta pisteiden
* vähennyksiltä vältyttäisiin.

* Teiden ja risteysten tietorakennekokonaisuus noudattelee varsin kiinteästi
* ensimmäisen osan rakenteita. Structit Crossroad ja way on koottu
* unordered_mappiin, kuten aiemminkin, sillä niitä ei ole syytä pitää kalliisti
* järjestyksessä koko ajan ja hakujen asymptoottinen tehokkuus on keskimääräisesti
* vakioarvoinen

* Alla olevat perustelut käyvät tähänkin valintaan




    Unordered mapit:

        std::unordered_map<PlaceID, std::shared_ptr<Place>> Places = {};
        std::unordered_map<AreaID, std::shared_ptr<Area>> Areas = {};

        Jotta voimme koota edellä kuvatut Area:t ja Place:t käytettäväksi
        kokonaisuudeksi, tulee ne koota johonkin säiliöön.

        Koska 1) alkioiden ei tarvitse olla järjestyksessä, vaan järjestely
        suoritetaan tilanteen vaatimalla tavalla, 2) jokaiselle alueelle sekä
        paikalle on omat, yksilöivät, "avainarvonsa" ja 3) olemme kiinnostuneita
        etsinnän, lisäyksen ja poiston tehokkuudesta, on perusteltua valita
        unordered_map eri paikat ja alueet kokoavaksi säiliöksi.

        Jaettu osoitin on datan turhan siirtelyn välttämiseksi, mutta kuitenkin
        globaalisti käytettäväksi. Myös viittausten poisto hoituu automaattisesti


* Structit:

*           struct Way
            {
              Distance pituus = 0;
              std::vector<Coord> coordinates = {};
            };

*
*           struct Crossroad
            {
                Distance matka = 0;
                Color vari = Color::WHITE;
                Coord pii;
                Coord myCoord;
                std::vector<std::pair<WayID, Coord>> waysTo = {};
            };

*   Crossroad structin läpikäymisen väri on tyypiltään enum:

*           enum class Color { WHITE, GRAY, BLACK };



Muut ratkaisut:

        Suurin osa metodeista ovat edellä kuvatuilla tietorakenteilla melko
        tehokkaasti käytettävissä hyvin suoralinjaisilla standardikirjaston
        algoritmeillä, mutta muutama ratkaisu lienee paikallaan selostaa
        tarkemmin. Tehtävässä on käytetty aikaisempien osakokonaisuuksien
        toteutuksia uusien osina.


        places_alphabetically. Paikkojen nimet tallennetaan mappiin
        (järjestettyyn) nimi avaimena, niin ovat luettavissa suoraan oikeassa
        järjestyksessä.


        Puun luonti all_subareas_in_area käskyssä on toteutettu siten, että
        rakenne käydään läpi rekursiivisesti funktiossa void
        makeLikeATree(AreaID). Joka nodella lisätään alueen tiedot
        luokan yksityiseen muuttujaan std::vector<AreaID> allSubareas_. Sieltä
        ne palautetaan rekursiivisen silmukan läpi käytyä. O (n)


        Siltä varalta, että coordinaattien etäisyyksien laskemista tarvitaan
        jatkossa vielä enemmänkin, on luotu metodi calcDistance, joka laskee
        etäisyyden pisteestä A, pisteeseen B.

        double calcDistance(const Coord& a, const Coord& b);

        Lähimpien paikkojen etsimiseen ja muuhunkin koordinaattien lajitteluun
        on luotu std::vector<PlaceID> sortCoord
        (const std::vector<PlaceID>& listToSort, const Coord& fromWhere)

        Parametreinä ovat vektori paikkaid:stä, jotka halutaan lajitella sekä
        lähtöpiste, josta etäisyys lasketaan. Structin operaattorin kanssa
        lajittelu tapahtuu sortilla O(nLog n).

        Noupeustesteistä. Find -operaatiot antavat N:n arvolla 300000 Timeoutin.
        Analyysi suoritettu find_places_name -metodille. Joka näyttää olevan
        lähinnä tehokkuutta O(nlog n), kuitenkin O(n):ään päin.

        Metodissa käydään läpi  kaikki alkiot n ja jokaisen alkion kohdalla
        suoritetaan unordered_map:istä alkion haku O(n) ≈ Θ(1) ja vektorin
        push_back O(1). Tämän laskelman mukaan tehokkuudeksi tulisi
        O(n^2) ≈ Θ(n). Tulokset sopinevat tehokkuuteen.


        N 	   cmds (sec) 	Cmds /     Cmds / 	Cmds /      Cmds /
                                n          (n log n)    n^2         (log n)
        10	4,65490E-03	4,65E-04    2,02E-04	4,65E-05    2,02E-03
        30	6,91397E-03	2,30E-04    6,78E-05	7,68E-06    2,03E-03
        100	1,46269E-02	1,46E-04    3,18E-05	1,46E-06    3,18E-03
        300	4,07290E-02	1,36E-04    2,38E-05	4,53E-07    7,14E-03
        1000	1,36058E-01	1,36E-04    1,97E-05	1,36E-07    1,97E-02
        3000	3,65873E-01	1,22E-04    1,52E-05	4,07E-08    4,57E-02
        10000	1,22108E+00	1,22E-04    1,33E-05	1,22E-08    1,33E-01
        30000	4,12468E+00	1,37E-04    1,33E-05	4,58E-09    4,00E-01
        100000	1,53083E+01	1,53E-04    1,33E-05	1,53E-09    1,33E+00

        MIN/MAX                 0,7967      0,5570      0,0034      0,0054
        RelStdDev               0,0861      0,2664      1,6326      1,5959







