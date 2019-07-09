#ifndef __SAMPLERMT_H__
#define __SAMPLERMT_H__

#include <vector>
#include <cassert>
#include "utilities/point2d.h"
#include "utilities/point3d.h"
#include "app/threadcontext.h"
#include "sampler.h"

template<class T>
class SamplerMT: public Sampler
{
public:
	SamplerMT();
	SamplerMT(int num_samples);
	SamplerMT(int num_samples, int num_sets);
	SamplerMT(const SamplerMT<T>& s);
	virtual ~SamplerMT();

public:
	SamplerMT<T>& operator= (const SamplerMT<T>& rhs);

public:
	virtual Point2D SampleUnitSquare();
	virtual Point2D SampleUnitDisk();
	virtual Point3D SampleHemisphere();
	virtual void MapSamplesToUnitDisk();
	virtual void MapSamplesToHemisphere(const float e);
	virtual Sampler* Clone() const;
	virtual void GenerateSamples();
	virtual void SetupShuffledIndices();

protected:
	std::vector<T*> m_samplers;
};

template<class T>
inline SamplerMT<T>::SamplerMT()
{
	int num_processes = ThreadContext::Instance().GetNumProcesses();
	m_samplers.resize(num_processes);
	for (int i = 0; i < num_processes; i++)
		m_samplers[i] = new T;
	GenerateSamples();
}

template<class T>
inline SamplerMT<T>::SamplerMT(int num_samples)
{
	int num_processes = ThreadContext::Instance().GetNumProcesses();
	m_samplers.resize(num_processes);
	for (int i = 0; i < num_processes; i++)
		m_samplers[i] = new T(num_samples);
	GenerateSamples();
}

template<class T>
inline SamplerMT<T>::SamplerMT(int num_samples, int num_sets)
{
	int num_processes = ThreadContext::Instance().GetNumProcesses();
	m_samplers.resize(num_processes);
	for (int i = 0; i < num_processes; i++)
		m_samplers[i] = new T(num_samples, num_sets);
	GenerateSamples();
}

template<class T>
inline SamplerMT<T>::SamplerMT(const SamplerMT<T>& s)
{
	int num_samplers = s.m_samplers.size();
	m_samplers.resize(num_samplers);
	for (int i = 0; i < num_samplers; i++)
		m_samplers[i] = dynamic_cast<T*>(s.m_samplers[i]->Clone());
}

template<class T>
inline SamplerMT<T>::~SamplerMT()
{
	for (size_t i = 0; i < m_samplers.size(); i++)
		delete m_samplers[i];
	m_samplers.clear();
}

template<class T>
inline SamplerMT<T>& SamplerMT<T>::operator=(const SamplerMT<T>& rhs)
{
	if (this == &rhs)
		return *this;

	for (int i = 0; i < m_samplers.size(); i++)
		delete m_samplers[i];
	m_samplers.clear();

	int num_samplers = s.m_samplers.size();
	m_samplers.resize(num_samplers);
	for (int i = 0; i < num_samplers; i++)
		m_samplers[i] = s.m_samplers[i]->clone();

	return *this;
}

template<class T>
inline Point2D SamplerMT<T>::SampleUnitSquare()
{
	int index = ThreadContext::Instance().GetCurrentThreadIndex();
	assert(static_cast<size_t>(index) < m_samplers.size());
	return m_samplers[index]->SampleUnitSquare();
}

template<class T>
inline Point2D SamplerMT<T>::SampleUnitDisk()
{
	int index = ThreadContext::Instance().GetCurrentThreadIndex();
	assert(static_cast<size_t>(index) < m_samplers.size());
	return m_samplers[index]->SampleUnitDisk();
}

template<class T>
inline Point3D SamplerMT<T>::SampleHemisphere()
{
	int index = ThreadContext::Instance().GetCurrentThreadIndex();
	assert(static_cast<size_t>(index) < m_samplers.size());
	return m_samplers[index]->SampleHemisphere();
}

template<class T>
inline void SamplerMT<T>::GenerateSamples()
{
	for (size_t i = 0; i < m_samplers.size(); i++)
		m_samplers[i]->GenerateSamples();
}

template<class T>
inline void SamplerMT<T>::SetupShuffledIndices()
{
	for (size_t i = 0; i < m_samplers.size(); i++)
		m_samplers[i]->SetupShuffledIndices();
}

template<class T>
void SamplerMT<T>::MapSamplesToUnitDisk()
{
	for (size_t i = 0; i < m_samplers.size(); i++)
		m_samplers[i]->MapSamplesToUnitDisk();
}

template<class T>
void SamplerMT<T>::MapSamplesToHemisphere(const float e)
{
	for (size_t i = 0; i < m_samplers.size(); i++)
		m_samplers[i]->MapSamplesToHemisphere(e);
}

template<class T>
Sampler* SamplerMT<T>::Clone() const
{
	return new SamplerMT<T>(*this);
}


#endif