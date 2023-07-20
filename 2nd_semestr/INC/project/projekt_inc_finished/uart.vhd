-- uart.vhd: UART controller - receiving part
-- Author(s): xplick04
--
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;

-------------------------------------------------
entity UART_RX is
port(
    CLK:         in std_logic;
    RST:         in std_logic;
    DIN:         in std_logic;
    DOUT:        out std_logic_vector(7 downto 0);
    DOUT_VLD:    out std_logic
);
end UART_RX;

-------------------------------------------------
architecture behavioral of UART_RX is
  
--inicializace promenych
signal READ_EN     : std_logic; --rozohduje, kde se zacina a konci cist
signal PRINT_EN    : std_logic; --znaci konec slova, nastavuje DOUT_VLD
signal cnt         : integer range 0 to 121; --pocitacka clocku, po 16 clock singalech se podle ni vypisuje,111+10 mist navic, pro rezervu
begin
  
    FSM: entity work.UART_FSM(behavioral)--namapovani portu z FSM
    port map (
        CLK 	     => CLK,
        RST 	     => RST,
        DIN 	     => DIN,
        READ_EN   => READ_EN,
        PRINT_EN  => PRINT_EN
    );
  
main: process(CLK, RST) begin --zacatek procesu, sensitivity list:CLK,RST
        if RST = '1' then--pokud prijde reset signal, resetuje se cnt
          cnt      <= 0;
        elsif rising_edge(CLK) then --pokud je nastupni hrana clocku
          
          if READ_EN = '1' then --pokud je povoleny read z FSM
            cnt <= cnt + 1; --pocita clock signal, pouze pokud je povolen read
            if cnt    = 0 then --zapise 1.(MSB)bit na output, chci zapisovat hned, jakmile je read_en(nachazim se na prvnim midbitu)
              DOUT(0) <= DIN;
            elsif cnt = 15 then --zapise 2. bit na output
              DOUT(1) <= DIN;
            elsif cnt = 31 then --zapise 3. bit na output
              DOUT(2) <= DIN;
            elsif cnt = 47 then --zapise 4. bit na output
              DOUT(3) <= DIN;
            elsif cnt = 63 then --zapise 5. bit na output
              DOUT(4) <= DIN;
            elsif cnt = 79 then --zapise 6. bit na output
              DOUT(5) <= DIN;
            elsif cnt = 95 then --zapise 7. bit na output
              DOUT(6) <= DIN;
            elsif cnt = 111 then --zapise 8.(LSB)bit na output
              DOUT(7) <= DIN;
            end if;  
          end if;
          
          if PRINT_EN = '1' then --pokud je print, konci slovo
            DOUT_VLD <= '1'; --nastavi signal, ktery znaci konec slova
            cnt      <= 0; --nastavi counter na 0
          else 
            DOUT_VLD <= '0';
          end if;
          
        end if;
        
  end process;
end behavioral;