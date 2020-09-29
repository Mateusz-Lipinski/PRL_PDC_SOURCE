Minimalne wymagania:

GNU Arm Embedded Toolchain / mingw // do kompilacji
Make for Windows  // do linkowania
STM32CubeMX - do konfiguracji i generowania templatki kodu
STM32 ST-LINK Utility - do programowania przez SWD i debugowania
stm32flash.exe - do programowania przez uart, binarka załączona w ./


Plik projektu CubeMX: ./stm32-vs.ioc

Kompilacja:
Makefile wygenerowany jest przez CubeMX. Przy tworzeniu nowego pliku .c trzeba go tam dodać
w cli: Make

Programowanie przez ST-Link:
flash.bat 
(flagi -c UR -HardRst są konieczne dla tego mikrokontrolera)


Programowanie przez uart:
flashuart.bat przy uruchomionym bootloaderze
(trzeba w pliku bat zmienić numer portu)

żeby uruchomić bootloader, trzeba NAJPIERW podłączyć kabel usb do komputera i do frontpanelu, a potem dopiero włączyć zasilacz,
przy założonej na złącze SWD zaślepce (ma ona za zadanie podtrzymać przez chwilę stan niski przy hardresecie mikrokontrolera)
Po zaprogramowaniu układu, żeby go uruchomić trzeba odpiąć usb i wyłączyć i włączyć zasilanie. 

Jeśli włożymy usb PO włączeniu zasilania, możemy komunikować się z działającym układem przez uart.

Podczas normalnego programowania wygodniej jest przez ST-LINKa - nie trzeba wyłączać i włączać zasilania i odpinać kabla przy każdym flashowaniu i dostępny jest debugger.

W README_framework.md opisałem dodatkowo skrypt w nodejs który pozwala na automatyczne buildowanie przy zmianie kodu, ale to opcjonalne.

Do komunikacji przez uart z commandline używam programu @serialport/terminal (dostępny w npm).
(wymagany npm i node, instalacja terminala przez `npm install @serialport/terminal -g`. Potem można uruchomić skrypt: `npm run serial` i wybrać port)

komendy uart są parsowane w `usart_utils.c`
kod komend zdefiniowany jest w `cmd.c`
 






