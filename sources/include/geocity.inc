#if defined _geocity_included
 #endinput
#endif
#define _geocity_included

static Handle:GeoCityDB = INVALID_HANDLE;
static Handle:GeoCityQuery = INVALID_HANDLE;

stock GeoipCity( const String:ip[], String:city[], const maxlength )
{
	new CanUseCity = GeoDatabaseConnect();
	
	if( CanUseCity )
	{
		new String:IP_Parts_[4][6];
		GeoStrPiece( ip, IP_Parts_, sizeof IP_Parts_, sizeof(IP_Parts_[]), '.' );
		
		new String:IP_Number_[28];
		GeoGetIpNum( IP_Parts_, IP_Number_, 28 );
		
		new String:Block_Table_[10];
		GeoGetTableBlock( IP_Number_, Block_Table_ );
		
		new Location_ID_ = GeoGetLocationID( IP_Number_, Block_Table_ );

		GeoGetCityName( Location_ID_, city, maxlength );
		
		CloseHandle( GeoCityQuery );
	}
	else
	{
		Format( city, maxlength, "" );
	}
}

static stock GeoGetCityName( const Location_ID, String:City_Name[], MaxNameLength )
{
	if( Location_ID != 0 )
	{	
		decl String:LocationQuery[120];
		
		Format( LocationQuery, 120, "SELECT city FROM Location WHERE locId = %d", Location_ID );
		
		GeoCityQuery = SQL_Query(GeoCityDB, LocationQuery);
		if( GeoCityQuery != INVALID_HANDLE )
		{
			SQL_FetchRow( GeoCityQuery );
			SQL_FetchString( GeoCityQuery, 0, City_Name, MaxNameLength );
			TrimString( City_Name );
			if( strlen( City_Name ) == 0 ) Format( City_Name, MaxNameLength, "Somewhere" );
			GeoCityQuery = INVALID_HANDLE;
		}				
	}
	else
	{
		Format( City_Name, MaxNameLength, "Somewhere" );
	}
}

static stock GeoGetLocationID( const String:IP_Number[], const String:Block_Table[] )
{
	new LocID = 0;
	decl String:BlocksQuery[256];
	Format( BlocksQuery, 256, "SELECT locId FROM '%s' WHERE startIpNum <= %s AND endIpNum >= %s", Block_Table, IP_Number, IP_Number ); 

	GeoCityQuery = SQL_Query(GeoCityDB, BlocksQuery);
	if( GeoCityQuery != INVALID_HANDLE )
	{
		if( SQL_FetchRow( GeoCityQuery ) )
		{
			LocID = SQL_FetchInt( GeoCityQuery, 0 );
		}
		GeoCityQuery = INVALID_HANDLE;
	}		
	return LocID;
}

static stock GeoGetTableBlock( const String:IP_Number[], String:Block_Table[] )
{
	decl String:Block_Table_Query[10][256];
	new isBlock = 0;
	
	Format( Block_Table_Query[0], 256, "SELECT %s >= 16777216 AND %s <= 1062184855",   IP_Number, IP_Number );
	Format( Block_Table_Query[1], 256, "SELECT %s >= 1062184856 AND %s <= 1127924703", IP_Number, IP_Number );
	Format( Block_Table_Query[2], 256, "SELECT %s >= 1127924704 AND %s <= 1255039599", IP_Number, IP_Number );
	Format( Block_Table_Query[3], 256, "SELECT %s >= 1255039600 AND %s <= 1367032863", IP_Number, IP_Number );
	Format( Block_Table_Query[4], 256, "SELECT %s >= 1367032864 AND %s <= 1469546495", IP_Number, IP_Number );
	Format( Block_Table_Query[5], 256, "SELECT %s >= 1469546496 AND %s <= 1582516215", IP_Number, IP_Number );
	Format( Block_Table_Query[6], 256, "SELECT %s >= 1582516216 AND %s <= 3167370679", IP_Number, IP_Number );
	Format( Block_Table_Query[7], 256, "SELECT %s >= 3167370680 AND %s <= 3391222239", IP_Number, IP_Number );
	Format( Block_Table_Query[8], 256, "SELECT %s >= 3391222240 AND %s <= 3566889359", IP_Number, IP_Number );
	Format( Block_Table_Query[9], 256, "SELECT %s >= 3566889360 AND %s <= 3741319167", IP_Number, IP_Number );
	
	for( new i = 1; i <= 10; i++ )
	{
		GeoCityQuery = SQL_Query(GeoCityDB, Block_Table_Query[i-1]);
		SQL_FetchRow( GeoCityQuery );
		isBlock = SQL_FetchInt( GeoCityQuery, 0 );
		if( isBlock == 1 )
		{
			Format( Block_Table, 10, "Blocks-%d", i );
			break;
		}
		GeoCityQuery = INVALID_HANDLE;				
	}
}

static stock GeoGetIpNum( const String:IP_Parts[4][6], String:IP_Number[], maxlength )
{
	decl String:IP_Number_Query[256];
	
	Format( IP_Number_Query, 256, "SELECT '' || ((16777216 * %s) + (65536 * %s) + (256 * %s) + %s)", IP_Parts[0], IP_Parts[1], IP_Parts[2], IP_Parts[3] ); 
	
	GeoCityQuery = SQL_Query(GeoCityDB, IP_Number_Query);
	if( GeoCityQuery != INVALID_HANDLE )
	{
		SQL_FetchRow( GeoCityQuery );
		SQL_FetchString( GeoCityQuery, 0, IP_Number, maxlength );
	}
	GeoCityQuery = INVALID_HANDLE;
}

static stock GeoDatabaseConnect( )
{
	if( FileExists( "addons/sourcemod/data/sqlite/GeoLiteCity.sq3" ) )
	{
		decl String:error[255];
		GeoCityDB = SQLite_UseDatabase( "GeoLiteCity", error, 255 );
		if( GeoCityDB != INVALID_HANDLE )
		{
			return true;
		}
	}
	return false;
}

static stock GeoStrPiece(const String:input[], String:output[][], outputsize, piecelen, token = '|')
{
    new i = -1, pieces, len = -1 ;
    
    while ( input[++i] != 0 )
    {
        if ( input[i] != token )
        {
            if ( ++len < piecelen )
                output[pieces][len] = input[i] ;
        }
        else
        {
            output[pieces++][++len] = 0 ;
            len = -1 ;
            
            if ( pieces == outputsize )
                return pieces ;
        }
    }
    return pieces + 1;
}
