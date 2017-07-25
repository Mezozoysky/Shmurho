#!/usr/bin/env perl

use strict;
use warnings;

use FindBin;
use lib "$FindBin::Bin/../lib";

use ShmurhoUtils;
use ShmurhoUtils::CmdLine;
use ShmurhoUtils::Props;

use Data::Dumper;
use Cwd 'cwd', 'abs_path';
use Carp;
use Scalar::Util qw/looks_like_number/;
use File::Basename;

$Carp::Verbose = 0;

my $usageText = qq{
Usage:
    shmurhogen.pl <project_name> [options]
        Will do the job
};

sub main
{
	#
	# Load config
	#

    my $props = ShmurhoUtils::Props->new();
    $props->load( "$FindBin::Bin/../cfg/shmurhogen.props" ) || carp( "Errors found while loading config" );

	#
	# Parse command line options and arguments
	#

    my $cmdLine = ShmurhoUtils::CmdLine->new();
    my @options =
    (
		#
		# Common options
		#
        ShmurhoUtils::CmdLine::Option->new(
            long => 'help'
            , short => 'h'
        )
        , ShmurhoUtils::CmdLine::Option->new(
            long => 'version'
            , short => 'v'
        )
        , ShmurhoUtils::CmdLine::Option->new(
            long => 'verbose'
            , short => 'V'
        )
		#
		# Project-related options
		#
        , ShmurhoUtils::CmdLine::Option->new(
            long => 'basedir'
            , short => 'b'
            , arg_type => ShmurhoUtils::CmdLine::Option::ARG_TYPE_PATH
        )
		, ShmurhoUtils::CmdLine::Option->new(
			long => 'template'
			, short => 't'
			, arg_type => ShmurhoUtils::CmdLine::Option::ARG_TYPE_STR
		)
		#
		# Modules-related options
		#
		, ShmurhoUtils::CmdLine::Option->new(
			long => 'app'
			, short => 'a'
		)
		, ShmurhoUtils::CmdLine::Option->new(
			long => 'lib'
			, short => 'l'
		)
		, ShmurhoUtils::CmdLine::Option->new(
			long => 'hol'
			, short => 'h'
		)
		, ShmurhoUtils::CmdLine::Option->new(
			long => 'module-app'
			, arg_type => ShmurhoUtils::CmdLine::Option::ARG_TYPE_LIST
		)
		, ShmurhoUtils::CmdLine::Option->new(
			long => 'module-lib'
			, arg_type => ShmurhoUtils::CmdLine::Option::ARG_TYPE_LIST
		)
		, ShmurhoUtils::CmdLine::Option->new(
			long => 'module-hol'
			, arg_type => ShmurhoUtils::CmdLine::Option::ARG_TYPE_LIST
		)
    );
    $cmdLine->addOptions( \@options );
	#$cmdLine->setArgsMin( 1 );
    $cmdLine->setArgsMax( 1 );

    $cmdLine->match( \@ARGV );

	#print( Data::Dumper::Dumper( 'CmdLine after matching <<< ', $cmdLine, '>>>' ) );
	#print( Data::Dumper::Dumper( 'Source options array after matching <<< ', @options, '>>>' ) );

	#
	# Handle common command line options
	#

	# Show version info and exit if --version
    if ( $cmdLine->isOptMatched( 'version' ) )
    {
        print( "shmurhogen.pl is a part of ShmurhoUtils $ShmurhoUtils::VERSION\n" );
		return ( 0 );
    }
	# Show usage info and exit if --help
    if ( $cmdLine->isOptMatched( 'help' ) )
    {
        print( $usageText );
        return ( 0 );
    }
	# Set verbosity
    my $verbose = $cmdLine->isOptMatched( 'verbose' );


	#
	# Handle command line arguments
	#

    my $argsCount = $cmdLine->getArgsCount();
	#my $projectName = "MyProject";
    if ( $argsCount > 0 )
    {
        $props->setProp( 'project.name', $cmdLine->getArg( 0 ) );
    }


	#
	# Handle other command line options
	#

    my $baseDir = cwd();
	if ( $cmdLine->isOptMatched( 'basedir' ) )
    {
        $baseDir = abs_path( $cmdLine->getOption( 'basedir' )->getMatchedArgToken() );
    }

	my $projectTemplate = "default";
	if ( $cmdLine->isOptMatched( 'template' ) )
	{
		$projectTemplate = $cmdLine->getOption( 'template' )->getMatchedArgToken();
	}
	
	#
	# Print requested options
	#
	
    if ( $verbose )
    {
		print( "Requested template: $projectTemplate\n" );
        print( "Requested project name: ", $props->getProp( 'project.name' ), "\n" );
        print( "Requested base dir: $baseDir\n" );
		#print( "Requested template: $projectTemplate\n" );
		#print( "Requested project name: $projectName\n" );
		#print( "Requested base dir: $baseDir\n" );
    }

	#print( Data::Dumper::Dumper( 'Props: <<<', $props->getProps(), '>>>' ) );
	#my $v2 = $props->getProp( 'artifact', deflt => 'default_v2' );



    $props->save( to => "$FindBin::Bin/../var/shmurhogen-output.props" );

    return ( 0 );
}

exit( main() );

__END__
