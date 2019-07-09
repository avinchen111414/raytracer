#include "viewplane.h"
#include "sampler/samplermt.h"
#include "sampler/jittered.h"

ViewPlane::ViewPlane()
	: 	m_hres(400), 
		m_vres(400),
		m_s(1.0),
		m_num_samples(1),
		m_gamma(1.0),
		m_inv_gamma(1.0),
		m_show_out_of_gamut(false),
		m_max_depth(1)
{
	SetSamples(m_num_samples);
}

ViewPlane::ViewPlane(const ViewPlane& vp)   
	:  	m_hres(vp.m_hres),  
		m_vres(vp.m_vres), 
		m_s(vp.m_s),
		m_num_samples(vp.m_num_samples),
		m_gamma(vp.m_gamma),
		m_inv_gamma(vp.m_inv_gamma),
		m_show_out_of_gamut(vp.m_show_out_of_gamut),
		m_max_depth(vp.m_max_depth)
{
	SetSamples(m_num_samples);
}

ViewPlane& 
ViewPlane::operator= (const ViewPlane& rhs) {
	if (this == &rhs)
		return (*this);
		
	m_hres 				= rhs.m_hres;
	m_vres 				= rhs.m_vres;
	m_s					= rhs.m_s;
	m_num_samples			= rhs.m_num_samples;
	m_gamma				= rhs.m_gamma;
	m_inv_gamma			= rhs.m_inv_gamma;
	m_show_out_of_gamut	= rhs.m_show_out_of_gamut;
	m_max_depth			= rhs.m_max_depth;
	
	if (m_sampler_ptr)
	{
		delete m_sampler_ptr;
		m_sampler_ptr = nullptr;
	}
	SetSamples(m_num_samples);
	return (*this);
}

ViewPlane::~ViewPlane(void) {}

void ViewPlane::SetSamples(const int n) {
	m_num_samples = n;

	if (m_sampler_ptr)
	{
		delete m_sampler_ptr;
		m_sampler_ptr = nullptr;
	}

	m_sampler_ptr = new SamplerMT<Jittered>(m_num_samples);
}

void ViewPlane::SetSampler(Sampler* sp)
{
	if (m_sampler_ptr)
	{
		delete m_sampler_ptr;
		m_sampler_ptr = nullptr;
	}

	m_num_samples = sp->GetNumSamples();
	m_sampler_ptr = sp;
}