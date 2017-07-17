package ShmurhoUtils::CmdLine;

=head1 NAME

ShmurhoUtils::CmdLine - command line arguments/options parser and storage

=cut

use 5.010;
use strict;
use warnings;

use ShmurhoUtils::CmdLine::Option;

use Carp;
use Scalar::Util qw/blessed/;


sub new
{
    my ( $class, %args ) = @_;
    my $self =
    {
        args_min => undef
        , args_max => undef
        , args_count => 0
    };

    return ( bless( $self, $class ) );
}

sub addOption
{
    my ( $self, $opt ) = @_;
    blessed( $opt ) eq 'ShmurhoUtils::CmdLine::Option' || croak "Option type error";
    push( @{ $self->{ opts } }, $opt );
}

sub addOptions
{
    my ( $self, $optsArrRef ) = @_;
    for ( my $i = 0; $i < scalar( @{ $optsArrRef } ); ++$i )
    {
        $self->addOption( @{ $optsArrRef }[ $i ] );
    }
}

sub getOption
{
    my ( $self, $name ) = @_;
    my $opts = $self->{ opts };
    my $found = undef;
    for ( my $i = 0; $i < scalar( @{ $opts } ); ++$i )
    {
        my $opt = @{ $opts }[ $i ];
        if ( $opt->getLongName() eq $name )
        {
            $found = $opt;
            last;
        }
    }
    return ( $found );
}

sub getOptions
{
    my ( $self ) = @_;
    return ( $self->{ opts } );
}

sub isOptMatched
{
    my ( $self, $name ) = @_;

    my $matched = 0;
    my $optsLen = scalar( @{ $self->{ opts } } );
    my $i = 0;
    for ( ; $i < $optsLen; ++$i )
    {
        my $opt = @{ $self->{ opts } }[ $i ];
        if ( $opt->getLongName() eq $name )
        {
            if ( $opt->isMatched() )
            {
                $matched = 1;
            }
            last;
        }
    }

    return ( $matched );
}

sub setArgsMin
{
    my ( $self, $min ) = @_;

    $self->{ args_min } = $min;
    if ( defined $self->{ args_max } && $self->{ args_max } < $min )
    {
        $self->{ arsg_max } = $min;
    }
}

sub setArgsMax
{
    my ( $self, $max ) = @_;

    $self->{ args_max } = $max;
    if ( defined $self->{ args_min } && $self->{ args_min } > $max )
    {
        $self->{ arsg_min } = $max;
    }
}

sub getArgsCount
{
    my ( $self ) = @_;
    return ( $self->{ args_count } );
}

sub getArgs
{
    my ( $self ) = @_;
    return ( $self->{ args } );
}

sub getArg
{
    my ( $self, $index ) = @_;

    my $indexMax = $self->{ args_count } - 1;
    if ( $index < 0 || $index > $indexMax )
    {
        croak( "Index $index is out of bounds [ 0, $indexMax ]" );
    }

    return ( @{ $self->{ args } }[ $index ] );
}

sub match
{
    my ( $self, $argvRef ) = @_;
    my $argc = scalar( @{ $argvRef } );

    if ( $argc > 0 )
    {
        while ( @{ $argvRef } )
        {
            my $token = shift( @{ $argvRef } );
            $self->_matchToken( $token, $argvRef );
        }
    }

}

sub _matchToken
{
    my ( $self, $token, $argvRef ) = @_;

    if ( $token eq '--' )
    {
        croak 'Not implemented';
#         return ( _matchSeparator() );
    }

    if ( $token =~ m/^--/ )
    {
        return ( $self->_matchLongOption( $token, $argvRef ) );
    }

    if ( $token =~ m/^-/ )
    {
        return ( $self->_matchShortOption( $token, $argvRef ) );
    }

    return ( $self->_matchArgument( $token, $argvRef ) );
}

# sub _matchSeparator
# {
# }

sub _matchLongOption
{
    my ( $self, $token, $argvRef ) = @_;

    my $optsLen = scalar( @{ $self->{ opts } } );
    my $i = 0;
    for ( ; $i < $optsLen; ++$i )
    {
        my $opt = @{ $self->{ opts } }[ $i ];
        my $long = $opt->getLongName();
        my $argType = $opt->getArgType();

        next unless ( $token eq "--$long" );

        $opt->setMatchedToken( $token );

        if ( $argType eq ShmurhoUtils::CmdLine::Option::ARG_TYPE_BOOL )
        {
            # Do nothing
        }
        else
        {
            my $argToken = shift( @{ $argvRef } ) || croak( "Option '$long' expect an argument of type '$argType'\n" );
            $opt->setMatchedArgToken( $argToken );

#             given ( $opt->getArgType() )
#             {
#                 when ( Option::ARG_TYPE_STR )
#                 {
#                 }
#             }
        }
        $opt->setMatched();
        last;
    }

    $i < $optsLen || croak( "No matching option found for token: '$token'" );
}

sub _matchShortOption
{
    my ( $self, $token, $argvRef ) = @_;

    my $optsLen = scalar( @{ $self->{ opts } } );
    my $i = 0;
    for ( ; $i < $optsLen; ++$i )
    {
        my $opt = @{ $self->{ opts } }[ $i ];
        my $short = $opt->getShortName();
        my $argType = $opt->getArgType();

        next unless ( $token eq "-$short" );

        $opt->setMatchedToken( $token );

        if ( $argType eq ShmurhoUtils::CmdLine::Option::ARG_TYPE_BOOL )
        {
            # do nothing
        }
        else
        {
            my $argToken = shift( @{ $argvRef } ) || croak( "Option '$short' expect an argument of type '$argType'\n" );
            $opt->setMatchedArgToken( $argToken );
        }
        $opt->setMatched();
        last;
    }

    $i < $optsLen || croak( "No matching option found for token: '$token'" );
}

sub _matchArgument
{
    my ( $self, $token, $argvRef ) = @_;

    if ( defined $self->{ args_max } && $self->{ args_max } <= $self->{ args_count } )
    {
        croak( "Cant match argument '$token', max arguments: $self->{ args_max };" );
    }
    push( @{ $self->{ args } }, $token );
    ++$self->{ args_count };
}


1;


=head1 BUGS


=head1 AUTHOR

Stanislav Demyanovich <mezozoysky@gmail.com>

=head1 LICENSE

This module may be distributed under the same terms as Perl itself.

=cut



