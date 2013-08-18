/* Flite_Plugin - for licensing and copyright see license.txt */

#include <StdAfx.h>
#include <CPluginFlite.h>
#include <MultiThread_Containers.h>

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

    /*
    const cst_val* segment_duration( const cst_item* seg )
    {
        const cst_item* s = item_as( seg, "Segment" );

        if ( !s )
        {
            return VAL_STRING_0;
        }

        else if ( item_prev( s ) == NULL )
        {
            return item_feat( s, "end" );
        }

        else
        {
            // It should be okay to construct this as it will get
            //       dereferenced when the CART interpreter frees its feature
            //       cache.
            return float_val( item_feat_float( s, "end" )
                              - item_feat_float( item_prev( s ), "end" ) );
        }
    } */

    float flite_text_to_speech_phenome( const char* text,
                                        cst_voice* voice,
                                        const char* outtype,
                                        void* pStream )
    {
        cst_utterance* u;

        float dur;
        float end_last = 0;
        float end_current = 0;
        float dur_current = 0;
        float dur_sum = 0;

        //feat_set_float( voice->features, "duration_stretch", 1 );

        u = flite_synth_text( text, voice );


        cst_item* s;
        string sRet;

        int nPhoneme = 0;

        for ( s = relation_head( utt_relation( u, "Segment" ) ); s; s = item_next( s ) )
        {
            SPhenomeTiming ps;

            string sPhoneme = item_feat_string( s, "name" );
            sRet += sPhoneme;

            end_current = item_feat_float( s, "end" );
            dur_current = end_current - end_last;

            //if ( !( nPhoneme == 0 && sPhoneme == "pau" ) )
            //{
            dur_sum += dur_current;
            //}

            ps.fWeight = 1;

            /* If its a vowel and is stressed output stress value */
            if ( ( cst_streq( "+", ffeature_string( s, "ph_vc" ) ) ) &&
                    ( cst_streq( "1", ffeature_string( s, "R:SylStructure.parent.stress" ) ) ) )
            {
                sRet += "1";
                ps.fWeight = 1.3;
            }

            sRet += " ";

            if ( pStream )
            {
                // fade into each other
                ps.sName = sPhoneme;
                ps.fStart = end_current - dur_current;
                ps.fEnd = end_current;
                ps.fDuration = dur_current;

                ( ( CryMT::queue<SPhenomeTiming>* )pStream )->push( ps );
            }

            end_last = end_current;
            ++nPhoneme;
        }

        dur = flite_process_output( u, outtype, FALSE );
        delete_utterance( u );

        return dur;
    }

    string getPhonemes( const char* sText )
    {
        string sRet;

        cst_features* args = new_features();
        cst_voice* v;
        cst_utterance* u;
        cst_item* s;
        const char* name;
        //const cst_val* d;

        flite_init();
        v = register_cmu_us_no_wave( NULL );

        u = flite_synth_text( sText, v );

        for ( s = relation_head( utt_relation( u, "Segment" ) ); s; s = item_next( s ) )
        {
            sRet += item_feat_string( s, "name" );
            float test = item_feat_float( s, "end" );
            //d = segment_duration( s );

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