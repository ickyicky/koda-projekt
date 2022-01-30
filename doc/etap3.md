---
title: "Dynamiczne kodowanie słownikowe LZSS"
subtitle: "Realizacja trzeciego etapu projektu"
author: [Domański Piotr, Pietrowcew Jakub, Skórka Kornel]
date: "2022-01-30"
titlepage: true
...

# Implementacja

Projekt został zaimplementowany w języku C++. Składa się z dwóch oddzielnych programów: kodera `coder_lzss` oraz dekodera `decode_lzss`. Oba programy posiadają jedynie interfejs konsolowy i są sterowane za pomocą parametrów ich wywołania.

## Wywołanie kodera

Argumentami wywołania kodera są:

- kodowany plik
- wstazanie na docelowy plik z zakodowanymi danymi

Efektem wywołania programu jest utworzenie docelowego pliku oraz pliku ze statystykami zawierającego:

- informację o wielkości słownika
- informację o wielkości bufora
- informację o czasie kodowania
- wielkość pliku przed kompresją
- wielkość pliku po kompresji
- stopień kompresji
- liczbę słów kodowych
- średnią długość bitową

## Wywołanie dekodera

Argumentami wywołania dekodera są:

- zakodowany plik
- wskazanie na docelowy plik ze zdekodowanymi danymi
- ewentualne wskazanie na plik ze statystykami

Efektem wywołania dekodera jest odkodowanie danych do wskazanego pliku docelowego. Dodatkowo, jeżeli został podany plik ze statystykami zostanie do niego dopisania informacja o czasie dekodowania.

# Kodowanie

# Dekodowanie

# Dane testowe

Program został przetestowany przy użyciu sztucznie wygenerowanych ciągów danych tekstowych i obrazów podach przez prowadzącego na stronie przedmiotu. Działanie algorytmu zostało sprawdzone pod kątem jakości kompresji (obliczanie stopnia kompresji) oraz czasu kompresji dekompresji danych.

Ponieważ LZSS jest algorytmem bezstratnej kompresji danych dodatkowo weryfikowaliśmy czy plik otrzymany po dekompresji skompresowanego pliku jest identyczny z oryginałem.

Do testów zostały wykorzystane pliki o rozkładzie równomiernym, normalnym oraz Laplace'a

## Histogramy danych testowych

## Entropie danych destowych

# Wyniki testowania

# Obserwacje i wnioski
