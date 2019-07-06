#ifndef __VIEWPLANE_H__
#define __VIEWPLANE_H__

class Sampler;

class ViewPlane
{
public:
	ViewPlane();   								// default Constructor
				
	ViewPlane(const ViewPlane& vp);				// copy constructor

	ViewPlane& operator= (const ViewPlane& rhs);		// assignment operator
		
	~ViewPlane();   							// destructor
						
	void 													
	SetHres(const int h_res);
	inline int GetHres() const { return m_hres; };
		
	void 													
	SetVres(const int v_res);
	inline int GetVres() const { return m_vres; };
				
	void
	SetPixelSize(const float size);
	inline float GetPixelSize() { return m_s; };
		
	void
	SetGamma(const float g);
	inline float GetGamma() const { return m_gamma; };
	inline float GetInvGamma() const { return m_inv_gamma; };
		
	void
	SetGamutDisplay(const bool show);
	inline bool GetGamutDisplay() const { return m_show_out_of_gamut; };
		
	void
	SetSamples(const int n);
	inline int GetNumSamples() { return m_num_samples; };

	void SetSampler(Sampler* sp);
	inline Sampler* GetSampler() { return m_sampler_ptr; };

	void SetMaxDepth(int max_depth);
	inline int GetMaxDepth() const { return m_max_depth; };

private:
	int 			m_hres;   					// horizontal image resolution 
	int 			m_vres;   					// vertical image resolution
	float			m_s;							// pixel size
	int				m_num_samples;				// number of samples per pixel
	Sampler*		m_sampler_ptr;				// pointer to some sampler	

	float			m_gamma;						// gamma correction factor
	float			m_inv_gamma;					// the inverse of the gamma correction factor
	bool			m_show_out_of_gamut;			// display red if RGBColor out of gamut
	int				m_max_depth;					// max recursive depth
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