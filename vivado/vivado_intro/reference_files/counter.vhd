library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity counter is
    port ( clk : in std_logic;
           reset : in std_logic;
           count : in std_logic;
           ctr_out : out std_logic_vector (3 downto 0));
end counter;

architecture behavioral of counter is
signal counter : unsigned(3 downto 0);
signal detect : unsigned(1 downto 0);
signal detected : std_logic;
begin

ctr: process (clk) --counter process
begin
   if clk='1' and clk'event then
      if reset='1' then 
         counter <= "0000";
      elsif detected='1' then
         counter <= counter + 1;
      end if;
   end if;
end process; 

edge: process (clk) --synchronize input signal
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

detected <= detect(0) and not detect(1); --detect rising edge
ctr_out <= std_logic_vector(counter);

end behavioral;
