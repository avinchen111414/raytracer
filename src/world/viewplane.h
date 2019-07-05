#ifndef __VIEWPLANE_H__
#define __VIEWPLANE_H__

class Sampler;

class ViewPlane
{
public:
	int 			m_hres;   					// horizontal image resolution 
	int 			m_vres;   					// vertical image resolution
	float			m_s;							// pixel size
	int				m_num_samples;				// number of samples per pixel
	Sampler*		m_sampler_ptr;				// pointer to some sampler	

	float			m_gamma;						// gamma correction factor
	float			m_inv_gamma;					// the inverse of the gamma correction factor
	bool			m_show_out_of_gamut;			// display red if RGBColor out of gamut
	int				m_max_depth;					// max recursive depth

public:
	ViewPlane();   								// default Constructor
				
	ViewPlane(const ViewPlane& vp);				// copy constructor

	ViewPlane& operator= (const ViewPlane& rhs);		// assignment operator
		
	~ViewPlane();   							// destructor
						
	void 													
	SetHres(const int h_res);
		
	void 													
	SetVres(const int v_res);
				
	void
	SetPixelSize(const float size);
		
	void
	SetGamma(const float g);
		
	void
	SetGamutDisplay(const bool show);	
		
	void
	SetSamples(const int n);

	void SetSampler(Sampler* sp);

	void SetMaxDepth(int max_depth);
};

inline void
ViewPlane::SetMaxDepth(int max_depth) {
	this->m_max_depth = max_depth;
}

// ------------------------------------------------------------------------------ set_hres

inline void 													
ViewPlane::SetHres(const int h_res) {
	m_hres = h_res;
}


// ------------------------------------------------------------------------------ set_vres

inline void 													
ViewPlane::SetVres(const int v_res) {
	m_vres = v_res;
}


// ------------------------------------------------------------------------------ set_pixel_size

inline void 													
ViewPlane::SetPixelSize(const float size) {
	m_s = size;
}


// ------------------------------------------------------------------------------ set_gamma

inline void
ViewPlane::SetGamma(const float g) {
	m_gamma = g;
	m_inv_gamma = (float)1.0 / m_gamma;
}


// ------------------------------------------------------------------------------ set_gamut_display

inline void
ViewPlane::SetGamutDisplay(const bool show) {
	m_show_out_of_gamut = show;
}

#endif