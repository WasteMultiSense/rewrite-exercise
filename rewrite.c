//Rewrite the following (intentionally terrible) code...
//transmit_next_msg() transmits data via UART depending on the state param.
//Depending on the state, it should transmit the respective message. 
//You do not need to transmit the null terminator on the end of the strings, just the stringy data bytes.
//Feel free to change anything at all - except the ret_t def, uart_tx() decl, and the contents of the strings to transmit.
//You do not need to compile it, but the resulting code should be quality that is ready-to-compile.

typedef enum
{
    SUCCESS = 0,
    FAIL
}ret_t;

/**@brief Transmits bytes out the serial UART port. Can only transmit 20 bytes at a time.
 *
 * @param data (input only) Ptr to data to be sent
 * @param size (input/output) Size of data to be sent. Updated with size of data that actually was sent on return.
 * @retval SUCCESS  If transaction was successful. Size param updated to valid Tx'd size.
 * @retval FAIL  If transaction failed. Size param invalid if it failed (assume 0 bytes sent).
 */
ret_t uart_tx(const char* data, size_t* size);

typedef enum
{
​    STATE_CONN_SETUP = 0, //Set up connection
    STATE_CONN_CHECK, //Check connection
    STATE_SEND_DATA, //Send data
} state_t;

ret_t transmit_next_msg(state_t state, char* data, size_t size)
{
    char* conn_setup[100] = "AT+CONNECT\r\n";
    char* conn_check[100] = "AT+CHECK\r\n";
    char* send_data[60] = "AT+DATA = 01234567890123456789";
    int ret = 0; //SUCCESS

    if (state == 0)
    {
        int size = sizeof(conn_setup);
        int result;
        result = uart_tx(conn_setup, &size);
        if (result == 0)
        {
            return SUCCESS;
        }
        else
        {
            return FAIL;
        }
    }
    else if (state == 1)
    {
        int size = sizeof(conn_check);
        
        uart_tx(conn_check, &size);
        return SUCCESS;
    }
    else //state == 2
    {
        int i = 20; //Can only send 20 bytes at a time
        
        uart_tx(send_data, &i);
        if (i < 20) return FAIL;
        uart_tx(&send_data[i], &i);
        if (i < 10) return FAIL;
        
        return SUCCESS;
    }
    
    return ret;
}
