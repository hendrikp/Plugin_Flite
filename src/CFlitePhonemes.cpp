/* Flite_Plugin - for licensing and copyright see license.txt */

#include <StdAfx.h>
#include <CPluginFlite.h>

extern "C"
{
#include <cst_args.h>
#include <flite.h>

#include <usenglish/usenglish.h>
#include <cmulex/cmu_lex.h>
};

namespace FlitePlugin
{
    static cst_voice* cmu_us_no_wave = NULL;

    static cst_utterance* no_wave_synth( cst_utterance* u )
    {
        return u;
    };

    cst_voice* register_cmu_us_no_wave( const char* voxdir )
    {
        cst_voice* v = new_voice();
        cst_lexicon* lex;

        v->name = "no_wave_voice";

        /* Set up basic values for synthesizing with this voice */
        usenglish_init( v );
        feat_set_string( v->features, "name", "cmu_us_no_wave" );

        /* Lexicon */
        lex = cmu_lex_init();
        feat_set( v->features, "lexicon", lexicon_val( lex ) );

        /* Intonation */
        feat_set_float( v->features, "int_f0_target_mean", 95.0 );
        feat_set_float( v->features, "int_f0_target_stddev", 11.0 );

        feat_set_float( v->features, "duration_stretch", 1.1 );

        /* Post lexical rules */
        feat_set( v->features, "postlex_func", uttfunc_val( lex->postlex ) );

        /* Waveform synthesis: diphone_synth */
        feat_set( v->features, "wave_synth_func", uttfunc_val( &no_wave_synth ) );

        cmu_us_no_wave = v;

        return cmu_us_no_wave;
    }

    string getPhonemes( const char* sText )
    {
        string sRet;

        cst_features* args = new_features();
        cst_voice* v;
        cst_utterance* u;
        cst_item* s;
        const char* name;

        flite_init();
        v = register_cmu_us_no_wave( NULL );

        u = flite_synth_text( sText, v );

        for ( s = relation_head( utt_relation( u, "Segment" ) ); s; s = item_next( s ) )
        {
            sRet += item_feat_string( s, "name" );

            /* If its a vowel and is stressed output stress value */
            if ( ( cst_streq( "+", ffeature_string( s, "ph_vc" ) ) ) &&
                    ( cst_streq( "1", ffeature_string( s, "R:SylStructure.parent.stress" ) ) ) )
            {
                sRet += "1";
            }

            sRet += " ";
        }

        delete_utterance( u );
        delete_features( args );

        return sRet;
    }

}