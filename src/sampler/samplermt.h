#ifndef __SAMPLERMT_H__
#define __SAMPLERMT_H__

#include <vector>
#include "utilities/point2d.h"
#include "app/threadcontext.h"

template<typename T>
class SamplerMT
{
public:
	SamplerMT<T>();
	SamplerMT<T>(int num_samples);
	SamplerMT<T>(int num_samples, int num_sets);
	SamplerMT<T>(const SamplerMT<T>& s);
	virtual ~SamplerMT<T>();

public:
	SamplerMT<T>& operator= (const SamplerMT<T>& rhs);

public:
	virtual Point2D SampleUnitSquare();
	virtual Point2D SampleUnitDisk();
	virtual Point2D SampleHemisphere();

protected:
	virtual void GenerateSamples();
	virtual void SetupShuffledIndices();

protected:
	int m_num_samples;
	int m_num_sets;
	std::vector<T*> m_samplers;
};

#endif

template<typename T>
inline SamplerMT<T>::SamplerMT()
{
	int num_processes = ThreadContext::Instance().GetNumProcesses();
	m_samplers.resize(num_processes);
	for (int i = 0; i < num_processes; i++)
		m_samplers[i] = new T;
}

template<typename T>
inline SamplerMT<T>::SamplerMT(int num_samples)
{
	int num_processes = ThreadContext::Instance().GetNumProcesses();
	m_samplers.resize(num_processes);
	for (int i = 0; i < num_processes; i++)
		m_samplers[i] = new T(num_samples);
}

template<typename T>
inline SamplerMT<T>::SamplerMT(int num_samples, int num_sets)
{
	int num_processes = ThreadContext::Instance().GetNumProcesses();
	m_samplers.resize(num_processes);
	for (int i = 0; i < num_processes; i++)
		m_samplers[i] = new T(num_samples, num_sets);
}

template<typename T>
inline SamplerMT<T>::SamplerMT(const SamplerMT<T>& s)
{
	int num_samplers = s.m_samplers.size();
	m_samplers.resize(num_samplers);
	for (int i = 0; i < num_samplers; i++)
		m_samplers[i] = s.m_samplers[i]->clone();
}

template<typename T>
inline SamplerMT<T>::~SamplerMT()
{
	for (int i = 0; i < m_samplers.size(); i++)
		delete m_samplers[i];
	m_samplers.clear();
}

template<typename T>
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

template<typename T>
inline Point2D SamplerMT<T>::SampleUnitSquare()
{
	int index = ThreadContext::Instance().GetCurrentThreadIndex();
	assert(index < m_samplers.size());
	return m_samplers[index]->sample_unit_square();
}

template<typename T>
inline Point2D SamplerMT<T>::SampleUnitDisk()
{
	int index = ThreadContext::Instance().GetCurrentThreadIndex();
	assert(index < m_samplers.size());
	return m_samplers[index]->sample_unit_disk();
}

template<typename T>
inline Point2D SamplerMT<T>::SampleHemisphere()
{
	int index = ThreadContext::Instance().GetCurrentThreadIndex();
	assert(index < m_samplers.size());
	return m_samplers[index]->sample_hemisphere();
}

template<typename T>
inline void SamplerMT<T>::GenerateSamples()
{
	for (int i = 0; i < m_samplers.size(); i++)
		m_samplers[i]->generate_samples();
}

template<typename T>
inline void SamplerMT<T>::SetupShuffledIndices()
{
	for (int i = 0; i < m_samplers.size(); i++)
		m_samplers[i]->setup_shuffled_indices();
}
