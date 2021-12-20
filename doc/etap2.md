---
title: "Dynamiczne kodowanie słownikowe LZSS"
subtitle: "Realizacja drugiego etapu projektu"
author: [Domański Piotr, Pietrowcew Jakub, Skórka Kornel]
date: "2021-11-20"
titlepage: true
...

# Wstępne wyniki implementacji

Zaimplementowaliśmy koder i dekoder LZSS, natomiast nie zaimplementowaliśmy słownika opartego o drzewo binarne (prowadzący wskazał, że jest to opcjonalne do implementacji). Kod źródłowy dostępny jest w [repozytorium github](https://github.com/ickyicky/koda-projekt).

Przygotowaliśmy skrypt tworzący histogramy i przy jego użyciu przygotowaliśmy histogramy podanych danych testowych.

# Wstępne wyniki testów algorytmu

Wstępnie przetestowaliśmy implementację na prostych plikach tekstowych z sukcesem zarówno przy kompresji jak i dekompresji. 
