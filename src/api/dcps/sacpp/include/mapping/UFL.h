#ifndef SACPP_MAPPING_UFL_H
#define SACPP_MAPPING_UFL_H

//#include "eOrb/mapping/base.h"


// Unbounded fixed-length sequence

template <class T, typename X> class DDS_DCPSUFLSeq
{
public:

   typedef T& _subscript_type;
   typedef const T& _const_subscript_type;

   // DDS_DCPS-Standard

   static T * allocbuf (DDS_DCPS::ULong nelems);
   static void freebuf (T * space);

   DDS_DCPSUFLSeq ();
   DDS_DCPSUFLSeq (DDS_DCPS::ULong);
   DDS_DCPSUFLSeq
   (
      DDS_DCPS::ULong max,
      DDS_DCPS::ULong len,
      T * data,
      DDS_DCPS::Boolean
      rel = 0
   );
   DDS_DCPSUFLSeq (const DDS_DCPSUFLSeq<T, X>&);
   ~DDS_DCPSUFLSeq ();

   DDS_DCPSUFLSeq<T, X>& operator = (const DDS_DCPSUFLSeq<T, X> &);

   DDS_DCPS::ULong maximum () const;
   void length (DDS_DCPS::ULong);
   DDS_DCPS::ULong length () const;

   T & operator[] (DDS_DCPS::ULong index);
   const T & operator[] (DDS_DCPS::ULong index) const;

   DDS_DCPS::Boolean release () const;

   void replace
   (
      DDS_DCPS::ULong max,
      DDS_DCPS::ULong length,
      T * data,
      DDS_DCPS::Boolean rel = 0
   );

   T * get_buffer (DDS_DCPS::Boolean orphan = 0);
   const T * get_buffer () const;

   // Proprietary extensions

   // for octet sequence only
   DDS_DCPSUFLSeq (const char *str);

private:

   DDS_DCPS::ULong m_max;
   DDS_DCPS::ULong m_length;
   DDS_DCPS::Boolean m_release;
   T * m_buffer;
};


template <class T, typename X> inline DDS_DCPSUFLSeq<T, X>::DDS_DCPSUFLSeq ()
:
   m_max (0),
   m_length (0),
   m_release (0),
   m_buffer (0)
{}

template <class T, typename X>
inline DDS_DCPSUFLSeq<T, X>::DDS_DCPSUFLSeq (DDS_DCPS::ULong max)
:
   m_max (max),
   m_length (0),
   m_release (0),
   m_buffer (0)
{
   if (m_max > 0)
   {
      m_buffer = allocbuf (m_max);
      m_release = 1;
   }
}

template <class T, typename X> inline DDS_DCPSUFLSeq<T, X>::DDS_DCPSUFLSeq
(
   DDS_DCPS::ULong max,
   DDS_DCPS::ULong length,
   T * data,
   DDS_DCPS::Boolean rel
)
:
   m_max (max),
   m_length (length),
   m_release (rel),
   m_buffer (data)
{
   assert (m_length <= m_max);
}

template <class T, typename X> inline DDS_DCPSUFLSeq<T, X>::DDS_DCPSUFLSeq
   (const DDS_DCPSUFLSeq<T, X>& that)
:
   m_max (0),
   m_length (0),
   m_release (0),
   m_buffer (0)
{
   *this = that;
}

template <class T, typename X> inline DDS_DCPSUFLSeq<T, X>::~DDS_DCPSUFLSeq()
{
   if (m_release)
   {
      freebuf (m_buffer);
   }
}

template <class T, typename X> inline DDS_DCPSUFLSeq<T, X>&
   DDS_DCPSUFLSeq<T, X>::operator = (const DDS_DCPSUFLSeq<T, X>& that)
{
   if (this != &that)
   {
      // allocate more buffer only if we don't have enough space

      if (that.m_max > m_max)
      {
         if (m_release)
         {
            freebuf (m_buffer);
         }

         m_buffer = allocbuf (that.m_max);
         m_release = 1;
      }
      m_max = that.m_max;
      m_length = that.m_length;

      // now copy the data

      if (m_length)
      {
         memcpy (m_buffer, that.m_buffer, m_length * sizeof (T));
      }
   }

   return *this;
}

template <class T, typename X> inline void DDS_DCPSUFLSeq<T, X>::replace
(
   DDS_DCPS::ULong max,
   DDS_DCPS::ULong length,
   T * data,
   DDS_DCPS::Boolean rel
)
{
   if (m_release)
   {
      freebuf (m_buffer);
   }

   m_max = max;
   m_length = length;
   m_buffer = data;
   m_release = rel;
}

template <class T, typename X> inline T * DDS_DCPSUFLSeq<T, X>::allocbuf
(
   DDS_DCPS::ULong nelems
)
{
   return new T [nelems];
}

template <class T, typename X> inline void DDS_DCPSUFLSeq<T, X>::freebuf (T * buffer)
{
   delete [] buffer;
}

template <class T, typename X>
inline DDS_DCPS::ULong DDS_DCPSUFLSeq<T, X>::maximum () const
{
   return m_max;
}

template <class T, typename X>
inline DDS_DCPS::ULong DDS_DCPSUFLSeq<T, X>::length () const
{
   return m_length;
}

template <class T, typename X>
inline DDS_DCPS::Boolean DDS_DCPSUFLSeq<T, X>::release () const
{
   return m_release;
}

template <class T, typename X>
inline void DDS_DCPSUFLSeq<T, X>::length (DDS_DCPS::ULong nelems)
{
   if (nelems > m_max)
   {
      T * oldBuf = m_buffer;

      m_max = nelems;
      m_buffer = allocbuf (m_max);

      if (m_length)
      {
         memcpy (m_buffer, oldBuf, m_length * sizeof (T));
      }

      if (m_release)
      {
         freebuf (oldBuf);
      }

      m_release = 1;
   }

   m_length = nelems;
}

template <class T, typename X>
inline T & DDS_DCPSUFLSeq<T, X>::operator[] (DDS_DCPS::ULong index)
{
   assert (index < m_length);
   return m_buffer[index];
}

template <class T, typename X>
inline const T & DDS_DCPSUFLSeq<T, X>::operator[] (DDS_DCPS::ULong index) const
{
   assert (index < m_length);
   return m_buffer[index];
}

template <class T, typename X>
inline T * DDS_DCPSUFLSeq<T, X>::get_buffer (DDS_DCPS::Boolean orphan)
{
   T * ret = nil;

   if (orphan)
   {
      if (m_release)
      {
         m_max = 0;
         m_length = 0;
         m_release = 1;
         ret = m_buffer;
         m_buffer = nil;
      }

      // if orphan && !m_release
      // return nil
   }
   else
   {
      ret = m_buffer;
   }

   return ret;
}

template <class T, typename X>
inline const T * DDS_DCPSUFLSeq<T, X>::get_buffer() const
{
   return m_buffer;
}

template <class T, typename X>
inline DDS_DCPSUFLSeq<T, X>::DDS_DCPSUFLSeq(const char *str)
:
   m_max (0),
   m_length (0),
   m_release (0),
   m_buffer (0)
{
   // This method is only valid for 1 byte types

   assert (sizeof (T) == 1);

   if (str && ((m_max = strlen(str)) > 0))
   {
      m_length = m_max;
      m_buffer = allocbuf (m_max);
      m_release = 1;
      memcpy (m_buffer, str, m_max);
   }
}

#endif /* SACPP_MAPPING_UFL_H */
