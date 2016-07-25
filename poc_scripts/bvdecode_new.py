
"""
Reads some data from BV board view file.

Run in PyWin.

Parts stolen from:
    http://stackoverflow.com/questions/25951912/oppposite-workings-of-oledb-odbc-between-python-and-ms-access-vba
"""

import win32com.client
import pyodbc

db = 'D:\\tmp\\MBP820-2054.bv'

def ado(query):
    '''
    connect with com dispatch objs
    '''
    conn = win32com.client.Dispatch(r'ADODB.Connection')
    DSN = ('PROVIDER = Microsoft.Jet.OLEDB.4.0;DATA SOURCE = ' + 'D:\\tmp\\MBP820-2054.bv' +  ';')
    conn.Open(DSN)

    rs = win32com.client.Dispatch(r'ADODB.Recordset')
    strsql = query
    rs.Open(strsql, conn, 1, 3)
    t = rs.GetRows()
    conn.Close()
    return t

queries = {
        'layout' : "select * from Layout;",
        'pin' : "select * from Pin;",
        'nail' : "select * from Nail;"
    }

if __name__ == '__main__':    
    out = 'D:\\tmp\\'

    print str(ado("select * from Pin;"))
    
