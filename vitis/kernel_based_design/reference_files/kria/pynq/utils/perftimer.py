# Author: F.Kesel
# Date: 19.06.2023

import time

class Timer:
    """
    A class used measure execution time of code

    Attributes
    ----------
    start_time : float
        start time
    message : str
        the message to be printed
    do_print_message: boolean
        if True then print messages
    accumulated_time : float
        accumulated time from each start-stop sequence
    elapsed_time : float
        elapsed time from last measurement
    is_running: boolean
        if timer is running = True, else False
    
    Methods
    -------
    start()
        sets the start time
    stop()
        stops and calculates the elapsed time and prints it
    get_accumulated_time()
        returns accumulated time
    reset_timer()
        reset timer
    """    
    def __init__(self, message, do_print_message):
        """
        Parameters
        ----------
        message : str
            The message to be printed with the time measurement
        do_print_message : int
            If the message should be printed (=1, else 0)
        """        
        self.start_time = 0.0
        self.accumulated_time = 0.0
        self.elapsed_time = 0.0
        self.is_running = False
        self.message = message
        self.do_print_message = do_print_message

    def start(self):
        """Sets the start time."""    
        if self.is_running is True:
            print(f"Timer is running. Use .stop() to stop it")
            return

        self.is_running = True
        self.start_time = time.perf_counter_ns()

    def stop(self):
        """
        Stops and calculates the elapsed time and prints it.
        The elapsed time is calculated in us. 
        Returns the elapsed time.
        """
        if self.is_running is False:
            print(f"Timer is not running. Use .start() to start it")
            return

        self.elapsed_time = (time.perf_counter_ns() - self.start_time)/1000
 
        self.accumulated_time = self.accumulated_time + self.elapsed_time
 
        if self.do_print_message is True:
            print(f"{self.message} elapsed time: {self.elapsed_time:0.3f} us")
           
        self.start_time = 0
        self.is_running = False
        return self.elapsed_time
    
    def get_accumulated_time(self):
        """Returns the accumulated time."""
        if self.do_print_message is True:
            print(f"{self.message} acumulated time: {self.accumulated_time:0.3f} us")
        return self.accumulated_time
    
    def get_elapsed_time(self):
        """Returns the accumulated time."""
        if self.do_print_message is True:
            print(f"{self.message} elapsed time: {self.elapsed_time:0.3f} us")
        return self.elapsed_time

    def reset_timer(self):
        """Reset the timer."""
        self.accumulated_time = 0.0
        self.elapsed_time = 0.0