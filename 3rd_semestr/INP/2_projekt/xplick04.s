; Vernamova sifra na architekture DLX
; Maxim Pli�ka xplick04

        .data 0x04          ; zacatek data segmentu v pameti
login:  .asciiz "xplick04"  ; <-- nahradte vasim loginem
cipher: .space 9 	    ; sem ukladejte sifrovane znaky (za posledni nezapomente dat 0)

        .align 2            ; dale zarovnavej na ctverice (2^2) bajtu
laddr:  .word login         ; 4B adresa vstupniho textu (pro vypis)
caddr:  .word cipher        ; 4B adresa sifrovaneho retezce (pro vypis)

        .text 0x40          ; adresa zacatku programu v pameti
        .global main        ; 
main:   ; sem doplnte reseni Vernamovy sifry dle specifikace v zadani
	
		addi r6, r6, 1		; r6 = 1
		lb r21, login(r6)	; r21 = login[1], sud� posun
		addi r6, r6, 1		; r6 = 2
		lb r1, login(r6)	; r1 = login[2], lich� posun

		sub r6, r6, r6    	; r6 = 0, pro indexaci od nuly		
		subi r21, r21, 96	; r21 = re�ln� abecedn� posun
		subi r1, r1, 96		; r1 = re�ln� abecedn� posun
while:
		lb r10, login(r6)	; r10 = login[r6]
		sgti r12, r10, 96   	; r12 = (r10 > 'a') 1 ? 0, jestli je r10 v�t�� = pokra�ov�n�, jinak to je ��slo
		bnez r12, continue	; pokud je r12 == 1, pokra�uje
		nop					;
		j end			; r12 je ��slo(r12 < 'a'), ukon�en� �ifrov�n�
		nop

continue:
		sub r12, r12, r12	; r12 = 0, vynulov�n� registru
		andi r12, r6, 1		; r6 AND "1", lich� = 1, sud� = 0 
		bnez r12, odd		; sko�� na "even", pokud je sud�
		nop

even:
		add r10, r10, r21	; r10 = r10 - r21, sud� posun
		sgti r12, r10, 122	; r12 = (r10 > 'z') 1 ? 0, pokud je r12 men�� ne� 'a'
		bnez r12, evenOver	; pokud r12 nen� validn� pro z�pis, sko�� na evenOver
		nop
		j whileEnd		; skok na z�pis
		nop

evenOver:
		subi r10, r10, 26	; r10 = r10 - 26, napraven� p�i p�ete�en�
		j whileEnd		; skok na z�pis
		nop

odd:	
		sub r10, r10, r1	; r10 = r10 - r1, lich� posun
		slti r12, r10, 97	; r12 = (r10 < 'a') 1 ? 0, pokud je r12 men�� ne� 'a'
		bnez r12, oddUnder	; pokud r12 nen� validn� pro z�pis, sko�� na oddUnder
		nop
		j whileEnd		; skok na z�pis
		nop

oddUnder:
		addi r10, r10, 26	; r10 = r10 + 26, napraven� p�i podte�en�
		j whileEnd		; skok na z�pis
		nop

whileEnd:	
		sb cipher(r6), r10  	; cipher[r6] = r10
		addi r6, r6, 1   	; r6 = r6 + 1
		j while		    	; skok na za��tek
		nop


end:    addi r14, r0, caddr ; <-- pro vypis sifry nahradte laddr adresou caddr
        trap 5  ; vypis textoveho retezce (jeho adresa se ocekava v r14)
        trap 0  ; ukonceni simulace
