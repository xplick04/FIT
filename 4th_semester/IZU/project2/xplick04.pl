% Zadání č. 11:
% Napište program, kterého úkol je daný predikátem u11(LIN,VIN,SOUT,LOUT), kde 
% LIN je vstupní celočíselný seznam s nejméně dvěma prvky, VIN je vstupní 
% proměnná obsahující přirozené číslo/práh, SOUT je výstupní seznam obsahující 
% všechna čísla seznamu LIN, které jsou menší nebo stejný jako práh, LOUT je 
% výstupní seznam obsahující všechna čísla seznamu LIN, která jsou větší než práh. 

% Testovací predikáty:                                  % SOUT  	LOUT
u11_1:- u11([5,27,-1,28,19,-4,10],10,SOUT,LOUT),
        write(SOUT),write(' '),write(LOUT).		% [5,-1,-4,10]	[27,28,19]
u11_2:- u11([5,27,-1,28,19,-4,10],28,SOUT,LOUT),
	write(SOUT),write(' '),write(LOUT).		% LIN           []		
u11_3:- u11([5,27,-1,28,19,-4,10],-8,SOUT,LOUT),
	write(SOUT),write(' '),write(LOUT).		% [] 		LIN
u11_r:- write('Zadej LIN: '),read(LIN),
        write('Zadej VIN: '),read(VIN),
        u11(LIN,VIN,SOUT,LOUT),
	write(SOUT),write(' '),write(LOUT).

u11(LIN,VIN,SOUT,LOUT):- lower(LIN, [], SOUT, VIN), higher(LIN, [], LOUT, VIN).

	lower([], LIST, LIST, _).	/*empty*/
	lower([H|T], LIST, [H|R], VIN) :- H =< VIN, lower(T, LIST, R, VIN). 
	lower([_|T], LIST, R, VIN) :- lower(T, LIST, R, VIN).

	higher([], LIST, LIST, _).	/*empty*/
	higher([H|T], LIST, [H|R], VIN) :- H > VIN, higher(T, LIST, R, VIN).
	higher([_|T], LIST, R, VIN) :- higher(T, LIST, R, VIN).
