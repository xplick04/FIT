-- cpu.vhd: Simple 8-bit CPU (BrainLove interpreter)
-- Copyright (C) 2021 Brno University of Technology,
--                    Faculty of Information Technology
-- Author(s): DOPLNIT
--

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity cpu is
 port (
   CLK   : in std_logic;  -- hodinovy signal
   RESET : in std_logic;  -- asynchronni reset procesoru
   EN    : in std_logic;  -- povoleni cinnosti procesoru
 
   -- synchronni pamet ROM
   CODE_ADDR : out std_logic_vector(11 downto 0); -- adresa do pameti
   CODE_DATA : in std_logic_vector(7 downto 0);   -- CODE_DATA <- rom[CODE_ADDR] pokud CODE_EN='1'
   CODE_EN   : out std_logic;                     -- povoleni cinnosti
   
   -- synchronni pamet RAM
   DATA_ADDR  : out std_logic_vector(9 downto 0); -- adresa do pameti
   DATA_WDATA : out std_logic_vector(7 downto 0); -- ram[DATA_ADDR] <- DATA_WDATA pokud DATA_EN='1'
   DATA_RDATA : in std_logic_vector(7 downto 0);  -- DATA_RDATA <- ram[DATA_ADDR] pokud DATA_EN='1'
   DATA_WREN  : out std_logic;                    -- cteni z pameti (DATA_WREN='0') / zapis do pameti (DATA_WREN='1')
   DATA_EN    : out std_logic;                    -- povoleni cinnosti
   
   -- vstupni port
   IN_DATA   : in std_logic_vector(7 downto 0);   -- IN_DATA obsahuje stisknuty znak klavesnice pokud IN_VLD='1' a IN_REQ='1'
   IN_VLD    : in std_logic;                      -- data platna pokud IN_VLD='1'
   IN_REQ    : out std_logic;                     -- pozadavek na vstup dat z klavesnice
   
   -- vystupni port
   OUT_DATA : out  std_logic_vector(7 downto 0);  -- zapisovana data
   OUT_BUSY : in std_logic;                       -- pokud OUT_BUSY='1', LCD je zaneprazdnen, nelze zapisovat,  OUT_WREN musi byt '0'
   OUT_WREN : out std_logic                       -- LCD <- OUT_DATA pokud OUT_WE='1' a OUT_BUSY='0'
 );
end cpu;


-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of cpu is

	--PC
		signal PC_DEC : std_logic;
		signal PC_INC : std_logic;
		signal PC_CODE_ADDR : std_logic_vector(11 downto 0);
	--PC
						
	--PTR
		signal PTR_DEC : std_logic;
		signal PTR_INC : std_logic;
		signal PTR_DATA_ADDR : std_logic_vector(9 downto 0);
	--PTR
	
	--MX
		signal MX_WDATA : std_logic_vector(7 downto 0);
		signal MX_SELECT : std_logic_vector(1 downto 0) := (others =>'0');
	--MX

	--FSM (s_start -> s_fetch -> s_decode -> operation -> s_fetch..)
		type fsm_state is(
			s_start,
			s_fetch,
			s_decode,
			
			s_ptr_inc,
			s_ptr_dec,
			
			s_cell_val_inc,
			s_cell_val_inc1,
			s_cell_val_dec,
			s_cell_val_dec1,
			s_cell_val_end,
			
			s_while_start,
			s_while_mid1,
			s_while_mid2,
			
			s_while_end,
			s_while_end2,
			s_while_end3,
			s_while_end4,
			
			s_cell_print,
			s_cell_print_done,
			s_cell_load,
			s_cell_load_done,
			
			s_break,
			s_break2,
			
			s_unknown,
			
			s_null
		);
		signal current_state : fsm_state := s_start;
		signal next_state    : fsm_state;
	--FSM
--------------------------------------------------------------------------		
begin
	--PC
		PC: process (CLK, RESET, PC_INC, PC_DEC)
		begin
			CODE_ADDR <= PC_CODE_ADDR;
			OUT_DATA <= DATA_RDATA;
			
			if RESET='1' then
				PC_CODE_ADDR <= (others => '0');
				
			elsif rising_edge(CLK) then
				if PC_INC = '1' then
					PC_CODE_ADDR <= PC_CODE_ADDR + 1;
				elsif PC_DEC = '1' then
					PC_CODE_ADDR <= PC_CODE_ADDR - 1;
				end if;
			end if;
			
		end process;		
	--PC
	
	
	--PTR
		PTR: process (CLK, RESET, PTR_INC, PTR_DEC)
		begin
			DATA_ADDR <= PTR_DATA_ADDR;
			
			if RESET='1' then
				PTR_DATA_ADDR <= (others => '0');
				
			elsif rising_edge(CLK) then
				if PTR_INC = '1' then
					PTR_DATA_ADDR <= PTR_DATA_ADDR + 1;
				elsif PTR_DEC = '1' then
					PTR_DATA_ADDR <= PTR_DATA_ADDR - 1;
				end if;		
			end if;
			
		end process;
	--PTR
	
	
	--MUX
		MUX: process (CLK, RESET, MX_SELECT)
		begin
			DATA_WDATA <= MX_WDATA;
		
			if RESET='1' then
				MX_WDATA <= (others => '0');
			elsif rising_edge(CLK) then
				case MX_SELECT is
				
					when "00" =>
						MX_WDATA <= IN_DATA;
					when "01" =>
						MX_WDATA <= DATA_RDATA + 1;
					when "10" =>
						MX_WDATA <= DATA_RDATA - 1;
					when others =>
						null;
				end case;
			end if;
		end process;		
	--MUX
	
	--FSM
		FSM_CURRENT_STATE: process (CLK, RESET, EN)
		begin
			if RESET = '1' then
				current_state <= s_start;
			elsif rising_edge(CLK) then
				if EN = '1' then
					current_state <= next_state;
				end if;
			end if;
		end process;
	--FSM
	
	--FSM
		FSM_NEXT_STATE: process (current_state, OUT_BUSY, IN_VLD, CODE_DATA, DATA_RDATA)
		begin

			PC_DEC    <= '0';
			PC_INC    <= '0';
			PTR_DEC   <= '0';
			PTR_INC   <= '0';
			CODE_EN   <= '0';
			IN_REQ    <= '0';
			DATA_EN   <= '0';
			DATA_WREN <= '0';
			MX_SELECT <= "00";
			OUT_WREN  <= '0';

--------------------------------------------------------------------------				
			case current_state is
			
				when s_start =>
					
					next_state<= s_fetch;
--------------------------------------------------------------------------						
				when s_fetch =>
					CODE_EN <= '1'; --povoleni cinnosti 
					
					next_state<= s_decode;
--------------------------------------------------------------------------						
				when s_decode =>				
						case CODE_DATA is
						
							when X"3E" => --">", inkremetnace hodnoty ukazatele
								next_state <= s_ptr_inc;
								
							when X"3C" => --"<", dekremetnace hodnoty ukazatele
								next_state <= s_ptr_dec;
								
							when X"2B" => --"+", inkrementace hodnoty aktulani bunky
								next_state <= s_cell_val_inc;
								
							when X"2D" => --"-", dekrementace hodnoty aktulani bunky
								next_state <= s_cell_val_dec;
								
							when X"5B" => --"[", zacatek while
								next_state <= s_while_start;
								
							when X"5D" => --"]", konec while cyklu
								next_state <= s_while_end;
								
							when X"2E" => --".", tisk hodnoty aktulani bunky
								next_state <= s_cell_print;
								
							when X"2C" => --",", nacteni hodnoty do aktualni bunky
								next_state <= s_cell_load;
								
							when X"7E" => --break, ukonci smycku while
								next_state <= s_break;
								
							when X"00" => --null, zastavi vykonavani programu
								next_state <= s_null;
								
							when others => --ostatni
								next_state <= s_unknown;
								
						end case;
--------------------------------------------------------------------------							
				when s_ptr_inc =>
					PTR_INC <= '1';
					PC_INC <= '1';
					
					next_state <= s_fetch;
					
				when s_ptr_dec =>
					PTR_DEC <= '1';
					PC_INC <= '1';
					
					next_state <= s_fetch;
--------------------------------------------------------------------------						
					
				when s_cell_val_inc =>
					DATA_EN   <= '1'; --DATA_RDATA <- ram[PTR], aktualizace DATA_RDATA podle hodnoty na adrese DATA_ADDR, povoleni cinnosti pameti
					DATA_WREN <= '0'; --DATA_RDATA <- ram[PTR], aktualizace DATA_RDATA podle hodnoty na adrese DATA_ADDR, povoleni cteni
					
					next_state<= s_cell_val_inc1;
					
				when s_cell_val_inc1 =>
					MX_SELECT <= "01"; --DATA_WDATA <= DATA_WDATA + 1
					
					next_state <= s_cell_val_end;
--------------------------------------------------------------------------						
				when s_cell_val_dec =>
					DATA_EN   <= '1'; --DATA_RDATA <- ram[PTR],aktualizace DATA_RDATA podle hodnoty na adrese DATA_ADDR, povoleni cinnosti pameti
					DATA_WREN <= '0'; --DATA_RDATA <- ram[PTR],aktualizace DATA_RDATA podle hodnoty na adrese DATA_ADDR, rezim cteni
					
					next_state <= s_cell_val_dec1;
					
				when s_cell_val_dec1 =>
					MX_SELECT <= "10"; --DATA_WDATA <= DATA_WDATA - 1
					
					next_state <= s_cell_val_end;
--------------------------------------------------------------------------						
				when s_cell_val_end =>
					DATA_EN   <= '1'; --ram[PTR] <- DATA_WDATA, povoleni cinnosti pameti
					DATA_WREN <= '1'; --ram[PTR] <- DATA_WDATA, rezim zapisu
					PC_INC <= '1';
					
					next_state <= s_fetch;
--------------------------------------------------------------------------					
					
				when s_cell_print =>
					DATA_EN   <= '1'; --DATA_RDATA <- ram[PTR],aktualizace DATA_RDATA podle hodnoty na adrese DATA_ADDR, povoleni cinnosti pameti
					DATA_WREN <= '0'; --DATA_RDATA <- ram[PTR],aktualizace DATA_RDATA podle hodnoty na adrese DATA_ADDR, rezim cteni
					
					next_state <= s_cell_print_done;
					
				when s_cell_print_done =>
					if OUT_BUSY = '1' then --while(OUT_BUSY), displej je zaneprazdnen, opakuje
						DATA_EN   <= '1';   --DATA_RDATA <- ram[PTR], aktualizace DATA_RDATA podle hodnoty na adrese DATA_ADDR, povoleni cinnosti pameti
						DATA_WREN <= '0';   --DATA_RDATA <- ram[PTR], aktualizace DATA_RDATA podle hodnoty na adrese DATA_ADDR, rezim cteni
						
						next_state <= s_cell_print_done;
					else
						OUT_WREN <= '1'; --OUT_DATA <= DATA_RDATA, rezim zapisu
						PC_INC <= '1';
						
						next_state <= s_fetch;
					end if;
--------------------------------------------------------------------------
				when s_while_start =>
					PC_INC <= '1';    --PC+1
					DATA_EN <= '1';   --DATA_RDATA <- ram[PTR], aktualizace DATA_RDATA podle hodnoty na adrese DATA_ADDR, povoleni cinnosti pameti
					DATA_WREN <= '0'; --DATA_RDATA <- ram[PTR], aktualizace DATA_RDATA podle hodnoty na adrese DATA_ADDR, rezim cteni		
					next_state <= s_while_mid1;
					
				when s_while_mid1 =>	
					if DATA_RDATA = "00000000" then --if(ram[PTR] == 0)
						CODE_EN <= '1';              --CODE_DATA = rom[PC]
						next_state <= s_while_mid2;
					else
						next_state <= s_fetch;
					end if;
					
				when s_while_mid2 =>
					if CODE_DATA = X"5D" then --until(rom[PC] == "]")
						PC_INC <= '1';         --PC+1
						next_state <= s_fetch; --while skonci
					else
						next_state <= s_while_mid1; --vraci se zpet
					end if;
--------------------------------------------------------------------------	
				when s_while_end =>	
					DATA_EN <= '1';   --DATA_RDATA <- ram[PTR]
					DATA_WREN <= '0'; --DATA_RDATA <- ram[PTR]		
					next_state <= s_while_end2;
					
				when s_while_end2 =>
					if DATA_RDATA /= "00000000" then --if(ram[PTR] != 0)
						PC_DEC <='1'; --PC-1
						next_state <= s_while_end3;
					else
						PC_INC <= '1'; --PC+1
						next_state <= s_fetch;
					end if;
					
				when s_while_end3 =>
					CODE_EN <= '1'; --CODE_DATA = rom[PC]
					next_state <= s_while_end4;
					
				when s_while_end4 =>
					if CODE_DATA = X"5B" then --until(rom[PC] == "[")
						PC_INC <= '1';         --PC+1
						next_state <= s_fetch;
					else
						next_state <= s_while_end2; --dokud se (rom[PC] != "["), jde zpet
					end if;
--------------------------------------------------------------------------	
				when s_break =>
					PC_INC <= '1';  --PC+1
					CODE_EN <= '1'; --CODE_DATA = rom[PC]
					next_state <= s_break2;
					
				when s_break2 =>
					if CODE_DATA = X"5D" then   --until(rom[PC] == "]")
						next_state <= s_fetch;
					else
						PC_INC <= '1';          --PC+1
						CODE_EN <= '1';         --CODE_DATA = rom[PC]
						next_state <= s_break2; --dokud se (rom[PC] != "]"), jde zpet
					end if;
--------------------------------------------------------------------------						
				when s_cell_load =>
					IN_REQ <= '1';     --pozadavek na data
					MX_SELECT <= "00"; --DATA_WDATA <= IN_DATA
					
					next_state<= s_cell_load_done;
						
				when s_cell_load_done =>
					if IN_VLD /= '1' then --while(!vld), ceka na data dokud IN_VLD = '1'
						IN_REQ <= '1';     --pozadavek na data
						MX_SELECT <= "00"; --DATA_WDATA <= IN_DATA
						
						next_state <= s_cell_load_done;
					else
						DATA_EN <= '1';   --ram[PTR] <= IN_DATA, hodnota bunky je aktulizovana hodnotou DATA_WDATA
						DATA_WREN <= '1'; --ram[PTR] <= IN_DATA, hodnota bunky je aktulizovana hodnotou DATA_WDATA
						PC_INC <= '1';
						
						next_state <= s_fetch;
					end if;	
--------------------------------------------------------------------------
				when s_null => --zacykli se az do konce
					next_state <= s_null;
					
				when s_unknown => --v pripade neznameno znaku se pricte PC
					PC_INC <= '1';
					next_state <= s_fetch;
					
				when others => 
					null;
					
			end case;
		
		end process;
	--FSM
	
end behavioral;
 

