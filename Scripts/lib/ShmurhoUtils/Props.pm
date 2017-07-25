package ShmurhoUtils::Props;

=head1 NAME

ShurhoUtils::Props - simple utility for loading/saving line-based properties

=cut

use 5.010;
use strict;
use warnings;

use Carp;
use Cwd 'cwd';
use Scalar::Util qw/looks_like_number/;

sub new
{
    my ( $class ) = @_;
    my $self = {};

    $self->{ re_prop_key } = qr/[\w\-+]+[\.\w\-+]+/;
    $self->{ re_str_val } = qr/[^"]*/;
    $self->{ re_num_val } = qr/\d+\.?\d*/;
    $self->{ re_prop_sepr } = ':';
    $self->{ re_key_sepr } = qr/\./;
    $self->{ re_sig_scalar } = qr/\$?/;
    $self->{ re_sig_array } = qr/\@/;
    $self->{ re_sig_hash } = qr/\%/;
    $self->{ re_sig_ref } = qr/\\/;

    return ( bless( $self, $class ) );
}

sub load
{
    my ( $self, $from ) = @_;

    my $success = $self->_parseFile( $from );
    $self->{ from } = $from;
    return ( $success );
}

sub save
{
    my ( $self, %opts ) = @_;

    my $to = $opts{ to } || $self->{ from } || croak( "Dont know where to save props" );
    my $currHash = $self->{ props };

    my @lines = ();
    $self->_encodeHash( $self->{ props }, \@lines, '' );

    my $fh;
    open( $fh, ">$to" ) || croak( "Cant open file '$to'" );
    for my $line ( @lines )
    {
        print $fh $line;
    }
    close( $fh );
}

sub _encodeHash
{
    my ( $self, $hashRef, $linesRef, $prefix ) = @_;

    my @hashKeys = keys( %{ $hashRef } );

    for my $hkey ( @hashKeys )
    {
        my $hval = $hashRef->{ $hkey };
        if ( ref( $hval ) eq ref( {} ) )
        {
            $self->_encodeHash( $hval, $linesRef, "$prefix$hkey." );
        }
        elsif ( ref( $hval ) eq ref( [] ) )
        {
            $self->_encodeArray( $hval, $linesRef, "\@$prefix$hkey" );
        }
        elsif ( looks_like_number( $hval ) )
        {
            $self->_encodeNumber( $hval, $linesRef, "\$$prefix$hkey" );
        }
        else
        {
            $self->_encodeString( $hval, $linesRef, "\$$prefix$hkey" );
        }
    }
}

sub _encodeArray
{
    my ( $self, $arrayRef, $linesRef, $key ) = @_;

    for my $elem ( @{ $arrayRef } )
    {
        if ( looks_like_number( $elem ) )
        {
            $self->_encodeNumber( $elem, $linesRef, "$key" );
        }
        else
        {
            $self->_encodeString( $elem, $linesRef, "$key" );
        }
    }
}

sub _encodeNumber
{
    my ( $self, $number, $linesRef, $key ) = @_;

    push( @{ $linesRef }, "$key : $number\n" );
}

sub _encodeString
{
    my ( $self, $str, $linesRef, $key ) = @_;

    push( @{ $linesRef }, "$key : \"$str\"\n" );
}

sub getProps
{
    my ( $self ) = @_;

    $self->{ props } = {} unless( exists( $self->{ props } ) );
    return  $self->{ props };
}

sub getProp
{
    my ( $self, $key, %opts ) = @_;
    my $deflt = $opts{ deflt } || undef;

    if ( ! exists( $self->{ props } ) )
    {
        carp( "Props are empty" );
        return ( $deflt );
    }

    my $ref = 0;
    my $sigil = '';
    if ( $key =~ /^($self->{re_sig_ref}?)([$self->{re_sig_array}$self->{re_sig_hash}$self->{re_sig_scalar}]?)($self->{re_prop_key})$/ )
    {
        $ref = 1 unless ( ! $1 );
        $sigil = $2;
        $key = $3;
#         print( "GET:: sigil: ''$sigil'', key: ''$key'';\n" );
    }
    else
    {
        croak( "Given key ''$key'' is invalid" ) unless ( $key =~ $self->{ re_prop_key } );
    }

    my $currHash = $self->{ props };
    my @subkeys = split( $self->{ re_key_sepr }, $key );
    my $keyLen = scalar( @subkeys );
    for ( my $i = 0; $i < $keyLen; ++$i )
    {
        my $subkey = $subkeys[ $i ];
        if ( $i == $keyLen - 1 ) # last subkey
        {
            if ( ! exists( $currHash->{ $subkey } ) )
            {
                my $str = 'Prop props';
                for ( my $j = 0; $j <= $i; ++$j )
                {
                    $str .= "->$subkeys[ $j ]";
                }
                $str .= ' doesnt exist';
                carp( $str );
                return ( $deflt );
            }

            if ( $sigil =~ $self->{ re_sig_array } )
            {
                carp( "Prop at given key ''$sigil$key'' is not an Array" ) unless ( ref( $currHash->{ $subkey } ) eq 'ARRAY' );
            }
            return ( $currHash->{ $subkey } );
        }
        else
        {
            if ( ! exists( $currHash->{ $subkey } ) )
            {
                my $str = 'Prop props';
                for ( my $j = 0; $j <= $i; ++$j )
                {
                    $str .= "->$subkeys[ $j ]";
                }
                $str .= ' doesnt exist';
                carp( $str );
                return ( $deflt );
            }
            $currHash = $currHash->{ $subkey };
        }
    }
}

sub setProp
{
	my ( $self, $key, $value, %opts ) = @_;

	$self->{props} = {} unless( exists( $self->{ props } ) );

    my $ref = 0;
    my $sigil = '';
    if ( $key =~ /^($self->{re_sig_ref}?)([$self->{re_sig_array}$self->{re_sig_hash}$self->{re_sig_scalar}]?)($self->{re_prop_key})$/ )
    {
        $ref = 1 unless ( ! $1 );
        $sigil = $2;
        $key = $3;
#         print( "GET:: sigil: ''$sigil'', key: ''$key'';\n" );
    }
    else
    {
        croak( "Given key ''$key'' is invalid" ) unless ( $key =~ $self->{ re_prop_key } );
    }

    my $currHash = $self->{ props };
    my @subkeys = split( $self->{ re_key_sepr }, $key );
    my $keyLen = scalar( @subkeys );
    for ( my $i = 0; $i < $keyLen; ++$i )
    {
        my $subkey = $subkeys[ $i ];
        if ( $i == $keyLen - 1 ) # last subkey
        {
            if ( ! exists( $currHash->{ $subkey } ) )
            {
                my $str = 'Prop props';
                for ( my $j = 0; $j <= $i; ++$j )
                {
                    $str .= "->$subkeys[ $j ]";
                }
                $str .= ' doesnt exist';
                carp( $str );
                return ( $deflt );
            }

            if ( $sigil =~ $self->{ re_sig_array } )
            {
                carp( "Prop at given key ''$sigil$key'' is not an Array" ) unless ( ref( $currHash->{ $subkey } ) eq 'ARRAY' );
            }
            return ( $currHash->{ $subkey } );
        }
        else
        {
            if ( ! exists( $currHash->{ $subkey } ) )
            {
                my $str = 'Prop props';
                for ( my $j = 0; $j <= $i; ++$j )
                {
                    $str .= "->$subkeys[ $j ]";
                }
                $str .= ' doesnt exist';
                carp( $str );
                return ( $deflt );
            }
            $currHash = $currHash->{ $subkey };
        }
    }
}

sub _parseFile
{
    my ( $self, $file ) = @_;

    my $success = 1;
    $self->{ line } = 0; # current line of props
    open( my $fh, '<', "$file" ) || croak( "Failed to open props file '$file'" );
    while ( <$fh> )
    {
        ++$self->{ line }; # start with 1
        if ( m/^\s*$/ ) # empty line
        {
            next;
        }
        if ( m/^\s*#.*$/ ) # comment line
        {
            next;
        }

        $success = 0 unless ( $self->_parsePropLine( $_ ) );
    }
    close( $fh );
    $self->{ line } = undef;
    return ( $success );
}

sub _parsePropLine
{
    my ( $self, $line ) = @_;

    chomp( $line );
    if ( $line =~ /^\s*($self->{re_sig_ref}?)([$self->{re_sig_array}$self->{re_sig_hash}$self->{re_sig_scalar}]?)($self->{re_prop_key})\s*$self->{re_prop_sepr}\s*(.*)$/ )
    {
        my $ref = $1 ? 1 : 0;
        my $sigil = $2;
        my $keyToken = $3;
        my $valToken = $4;

#         print( "PARSE:: sigil: ''$sigil'', key: ''$keyToken'';\n" );

        # parse value
        my $value = undef;

        if ( $valToken =~ /^"($self->{ re_str_val })"$/ )
        {
            $value = $1;
        }
        elsif ( $sigil !~ $self->{ re_sig_hash } &&  $valToken =~ /^($self->{ re_num_val })$/ )
        {
            $value = $1;
        }
        else
        {
            carp( "Failed to parse value token ''$valToken''" );
            return ( 0 );
        }

        croak( "Value must be defined" ) unless ( defined( $value ) );

        # add key
        my @subkeys = split( /$self->{ re_key_sepr }/, $keyToken );
        my $keyLen = scalar( @subkeys );
        croak( "Failed to parse prop key ''$keyToken'' on line #$self->{ line }" ) unless ( $keyLen > 0 );
        $self->{ props } = {} unless ( exists( $self->{ props } ) );
        my $currHash = $self->{ props };
        for ( my $i = 0; $i < $keyLen; ++$i )
        {
            my $subkey = $subkeys[ $i ];
            if ( $i == $keyLen - 1 ) # last subkey
            {
                if ( $sigil =~ $self->{ re_sig_array } )
                {
                    $currHash->{ $subkey } = [] unless ( exists( $currHash->{ $subkey } ) );
                    push( @{ $currHash->{ $subkey } }, $value );
                }
                elsif ( $sigil =~ $self->{ re_sig_hash } )
                {
                    croak( "Hash prop isnt implemented yet" );
                }
                elsif ( $sigil =~ $self->{ re_sig_scalar } )
                {
                    if ( exists( $currHash->{ $subkey } ) )
                    {
                        carp( "Scalar prop ''$keyToken'' duplicated on line #$self->{ line } (last value will be used) " );
                    }
                    $currHash->{ $subkey } = $value;
                }
            }
            else
            {
                $currHash->{ $subkey } = {} unless ( exists( $currHash->{ $subkey } ) );
                $currHash = $currHash->{ $subkey };
            }

        }

        return ( 1 );
    }
    else
    {
        carp( "Failed to parse line #$self->{ line } ''", $line, "''\n" );
        return ( 0 );
    }
}

1;


=head1 BUGS


=head1 AUTHOR

Stanislav Demyanovich <mezozoysky@gmail.com>

=head1 LICENSE

This module may be distributed under the terms of Zlib/libpng license.

=cut

