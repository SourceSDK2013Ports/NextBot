// NextBotAttentionInterface.cpp
// Manage what this bot pays attention to
// Author: Michael Booth, April 2007
//========= Copyright Valve Corporation, All rights reserved. ============//

#include "cbase.h"

#include "NextBot.h"
#include "NextBotAttentionInterface.h"
#include "NextBotBodyInterface.h"

#include "tier0/vprof.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


//------------------------------------------------------------------------------------------
/**
 * Reset to initial state
 */
void IAttention::Reset( void )
{
	m_body = GetBot()->GetBodyInterface();

	m_attentionSet.RemoveAll();
}


//------------------------------------------------------------------------------------------
/**
 * Update internal state
 */
void IAttention::Update( void )
{
}


//------------------------------------------------------------------------------------------
void IAttention::AttendTo( CBaseEntity *who, const char *reason )
{
	if ( !IsAwareOf( who ) )
	{
		PointOfInterest p;
		p.m_type = PointOfInterest::ENTITY;
		p.m_entity = who;
		p.m_duration.Start();

		m_attentionSet.AddToTail( p );
	}
}


//------------------------------------------------------------------------------------------
void IAttention::AttendTo( const Vector &where, IAttention::SignificanceLevel significance, const char *reason )
{
	PointOfInterest p;
	p.m_type = PointOfInterest::POSITION;
	p.m_position = where;
	p.m_duration.Start();

	m_attentionSet.AddToTail( p );
}


//------------------------------------------------------------------------------------------
void IAttention::Disregard( CBaseEntity *who, const char *reason )
{
	FOR_EACH_VEC( m_attentionSet, it )
	{
		if ( m_attentionSet[ it ].m_type == PointOfInterest::ENTITY )
		{
			CBaseEntity *myWho = m_attentionSet[ it ].m_entity;

			if ( !myWho || myWho->entindex() == who->entindex() )
			{
				m_attentionSet.Remove( it );
				return;
			}
		}
	}
}


//------------------------------------------------------------------------------------------
/**
 * Return true if given actor is in our attending set
 */
bool IAttention::IsAwareOf( CBaseEntity *who ) const
{
	FOR_EACH_VEC( m_attentionSet, it )
	{
		if ( m_attentionSet[ it ].m_type == PointOfInterest::ENTITY )
		{
			CBaseEntity *myWho = m_attentionSet[ it ].m_entity;

			if ( myWho && myWho->entindex() == who->entindex() )
			{
				return true;
			}
		}
	}

	return false;
}