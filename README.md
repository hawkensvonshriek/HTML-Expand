# HTML-Expand
A JSON-based configuration language for static HTML websites

# Command line usage
expand config.json /path/to/HTML/repo /path/to/output/dir defaults.json

# Configuration file
## Key flags
### -f -- specifies filename of an HTML template (without extension) in the supplied repository directory path
### -D -- add defaults from specified defaults subtree to current object level

## Value flags
### -d -- copies value from defaults configuration

# Notes
You may run into seg faults or other such atrocities. Please notify me or fork it or whatever.
