package ShmurhoUtils::CmdLine::Option;

=head1 NAME

ShmurhoUtils::CmdLine::Option - class represents a command line option.

=cut

use strict;
use warnings;

use Carp;

use constant
{
    ARG_TYPE_BOOL   => 'Bool'
    , ARG_TYPE_STR  => 'String'
    , ARG_TYPE_PATH => 'Path'
    , ARG_TYPE_INT  => 'Int'
    , ARG_TYPE_LIST => 'List'
    , ARG_TYPE_CSV  => 'CSV'
};

sub new
{
    my ( $class, %args ) = @_;
    my $self = {
        long => $args{ long } || croak( 'At least \'long\' must be specified for construction.' )
        , short => $args{ short } || undef
        , arg_type => $args{ arg_type } || ARG_TYPE_BOOL
        , required => $args{ required } && ( $args{ required } eq 'yes' && 1 || 0  ) || 0
    };

    return ( bless( $self, $class ) );
}

sub getLongName
{
    my ( $self ) = @_;
    return ( $self->{ long } );
}

sub getShortName
{
    my ( $self ) = @_;
    return ( $self->{ short } );
}

sub getArgType
{
    my ( $self ) = @_;
    return ( $self->{ arg_type } );
}

sub isRequired
{
    my ( $self ) = @_;
    return ( $self->{ required } );
}

sub setMatched
{
    my ( $self ) = @_;
    $self->{ matched } = 1;
}

sub setUnmatched
{
    my ( $self ) = @_;
    if ( defined $self->{ matched } )
    {
        $self->{ matched } = 0;
    }
}

sub isMatched
{
    my ( $self ) = @_;
    return ( $self->{ matched } || 0 );
}

sub setMatchedToken
{
    my ( $self, $token ) = @_;
    $self->{ token } = $token;
}

sub getMatchedToken
{
    my ( $self ) = @_;
    return ( $self->{ token } || '' );
}

sub setMatchedArgToken
{
    my ( $self, $token ) = @_;
    $self->{ arg_token } = $token;
}

sub getMatchedArgToken
{
    my ( $self ) = @_;
    return ( $self->{ arg_token } || '' );
}


1;


=head1 BUGS


=head1 AUTHOR

Stanislav Demyanovich <mezozoysky@gmail.com>

=head1 LICENSE

This module may be distributed under the terms of Zlib/libpng license

=cut
