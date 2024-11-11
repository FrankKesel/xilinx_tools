# Author: F.Kesel
# Date: 19.06.2023

def run_ip(ip):
    """Start the IP core and wait until it is done  """
    # Set AP_CONTINUE
    ip.register_map.CTRL.AP_CONTINUE = 1
    # Start IP Core
    ip.register_map.CTRL.AP_START = 1
    # Check AP_DONE flag and proceed if it is set
    while True:
        status = ip.register_map.CTRL.AP_DONE
        if status == 1:
            break