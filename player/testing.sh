###### incorrect #s of arguments ######
# zero arguments                                                                    
./entry

# one argument                                                                      
./entry 8088

# three arguments
./entry 8088 10 100

##### invalid arguments ######
# non-integer port number
./entry WRONG 10

# non-integer numberofrounds
./entry 8088 WRONG