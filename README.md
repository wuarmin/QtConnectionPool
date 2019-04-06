# QtConnectionPool

Simple Database Connection Pool for Qt 5 Applications 

## Getting Started

### Configuration

The connection pool can be configured in a json file.

#### Configuration example
```json
{
  "connectionPool": {
    "checkInterval": 30000,
    "minConnections": 2,
    "maxConnections": 7,
    "connectionLifePeriod": 600000,
    "database": {
        "driver": "OCI",
        "host": "happy-host.com",
        "port": 9999,
        "database": "happy_db",
        "user": "forrest_gump",
        "password": "xxxyyy"
    }
  }
}

```

#### Options

- **checkInterval**: The used interval in milliseconds to clean up exceeded connections of refresh open connections. Defaults to `10000`
- **minConnections**: The minimal count of open connections. Defaults to `1`
- **maxConnections**: The maximal count of open connections. Defaults to `1`
- **connectionLifePeriod**: The maximal life period in milliseconds of an open connection. Defaults to `300000

