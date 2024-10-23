library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity counter is
    Port ( clk : in STD_LOGIC;
           reset : in STD_LOGIC;
           count : in STD_LOGIC;
           ctr_out : out STD_LOGIC_VECTOR (3 downto 0));
end counter;

architecture Behavioral of counter is
signal counter : unsigned(3 downto 0);
signal detect : unsigned(1 downto 0);
signal detected : std_logic;
begin

ctr: process (clk) --Counter process
begin
   if clk='1' and clk'event then
      if reset='1' then 
         --counter <= (others => '0');
         counter <= "0000";
      elsif detected='1' then
         counter <= counter + 1;
      end if;
   end if;
end process; 

edge: process (clk) --Synchronize input signal
begin
   if clk='1' and clk'event then
      if reset='1' then 
        detect <= "00";
      else
        detect(0) <= count;
        detect(1) <= detect(0);
      end if;
   end if; 
end process;

detected <= detect(0) and not detect(1); --Detect rising edge
ctr_out <= std_logic_vector(counter);

end Behavioral;
