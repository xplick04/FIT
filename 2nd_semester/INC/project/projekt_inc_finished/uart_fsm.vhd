-- uart_fsm.vhd: UART controller - finite state machine
-- Author(s): xplick04
--
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all; --na inkrementaci cnt, cnt bit
-------------------------------------------------
entity UART_FSM is
port(
   CLK      : in std_logic;
   RST      : in std_logic;
   DIN      : in std_logic;
   READ_EN  : out std_logic;
   PRINT_EN : out std_logic
   );
end entity UART_FSM;

-------------------------------------------------
architecture behavioral of UART_FSM is
  
-- inicializace promenych
signal cnt         : integer range 0 to 24; --pocitacka clock singalu
signal cnt_bit     : integer range 0 to 8; --pocitacka na bity slova
signal word_in     : std_logic := '0'; --signal pro nacteny celeho slova   
type state_type is (S_WAIT_START_BIT, S_START_BIT, S_RECEIVE_DATA); --stavy, kterych muze FSM nabyt
signal state: state_type := S_WAIT_START_BIT; --pocatecni stav FSM

--specifikace stavu:
--S_WAIT_START_BIT = cekam nez mi prijde start bit nebo jsem za nactenym slovem
--S_START_BIT = jsem na startbitu
--S_RECIEVE_DATA = nacitam bity slova

begin --behavioral

  main: process(CLK, RST) begin --zacatek procesu, sensitivity list:CLK,RST
    
    if RST = '1' then--pokud prijde reset signal, resetuji se vsechnyhodnoty a ceka se na start bit
      state    <= S_WAIT_START_BIT;
      cnt      <= 0;
      cnt_bit  <= 0;
      word_in  <= '0'; 
      READ_EN  <= '0';
      PRINT_EN <= '0';
    
    elsif rising_edge(CLK) then--clock event
      
      if state = S_RECEIVE_DATA then--nastavi read, pokud je povoleno cteni
        READ_EN <= '1';
      else
        READ_EN <= '0';
      end if; 
    
      if word_in = '1' then --pokud je nacteno cele slovo, vysle se 1 clock dlouhy signal pro print
        PRINT_EN     <= '1';
        word_in      <= '0';
      else
        PRINT_EN <= '0'; 
      end if;
      
      if state = S_WAIT_START_BIT then --po prichodu start bitu prepne na stav a pripocita do pocitani clocku
        if DIN = '0' then
          cnt   <= cnt + 1;
          state <= S_START_BIT;
        end if;
      end if;
        
      if state = S_START_BIT then
        cnt <= cnt + 1;
        if cnt = 23 then --jsme uprostred prvniho bitu slova, potrebujeme prepnout stav na recieve
          state <= S_RECEIVE_DATA;
          cnt   <= 0; --vynulujeme count, pote pocitame jen do 16(od midbitu do dalsiho midbitu)
        end if;
      end if;
      
      if state = S_RECEIVE_DATA then
        cnt <= cnt + 1;
        if cnt = 15 then --pokud jsme na 16 bitu ve slove, pricteme do pocitace bitu ve slove
          cnt_bit <= cnt_bit + 1;
          cnt     <= 0;
        end if;
        if cnt_bit = 7 then --pokud jsme na poslednim bitu slova
          if cnt = 8 then --az budeme za slovem(vcetne stopbitu)spustime print signal, vynulujeme citacku clocku a stav se nastavi na puvodni(ready 2 go again)
            cnt_bit      <= 0;
            state        <= S_WAIT_START_BIT;
            cnt          <= 0;
            word_in      <= '1';
          end if;
        end if;
      end if;
      
    end if; --end clock processu
  
  end process; --end main process
   
end behavioral;