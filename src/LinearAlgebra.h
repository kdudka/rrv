/*
 * Copyright (C) 2015 Claude Heiland-Allen <claude@mathr.co.uk>
 *
 * This file is part of rrv (Radiosity Renderer and Visualizer).
 *
 * rrv is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * rrv is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with rrv.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LINEARALGEBRA_H
#define LINEARALGEBRA_H

#include <cassert>
#include <cstddef>              // for size_t

template<typename A>
class DenseVector
{
    private:
        int count_;
        A *array_;

    public:
        DenseVector(int count)
        : count_(count)
        {
            array_ = new A[count_];
        }

        DenseVector(const DenseVector<A> &b)
        : count_(b.count_)
        {
            array_ = new A[count_];
            for(int i = 0; i < count_; ++i)
                array_[i] = b.array_[i];
        }

        ~DenseVector()
        {
            delete[] array_;
        }

        inline int count() const
        {
            return count_;
        }

        void clear(const A &a)
        {
            for(int i = 0; i < count_; ++i)
                array_[i] = a;
        }

        inline const A &operator[](int ix) const
        {
            assert(0 <= ix);
            assert(ix < count_);
            return array_[ix];
        }

        inline A &operator[](int ix)
        {
            assert(0 <= ix);
            assert(ix < count_);
            return array_[ix];
        }

        template<typename B>
        B dot(const DenseVector<B> &b) const
        {
            assert(count_ == b.count());
            B acc;
            for(int j = 0; j < count_; ++j)
              acc += b[j] * array_[j];
            return acc;
        }

        int sparsity(const A &threshold) const
        {
            int j = 0;
            for(int i = 0; i < count_; ++i)
                if (array_[i] > threshold)
                    ++j;
            return j;
        }

        size_t size() const
        {
            return count_ * sizeof(A);
        }

        static int size(int n)
        {
            return n * sizeof(A);
        }
};

template<typename A>
class SparseVector
{
    private:
        int count_;
        int *key_;
        A *value_;

    public:
        SparseVector(const DenseVector<A> &dense, const int &count, const A &threshold)
        : count_(count)
        {
            key_ = new int[count_];
            value_ = new A[count_];
            int n = dense.count();
            int j = 0;
            for(int i = 0; i < n; ++i)
                if (dense[i] > threshold)
                {
                    assert(j < count_);
                    key_[j] = i;
                    value_[j] = dense[i];
                    ++j;
                }
            assert(j == count_);
        }

        ~SparseVector()
        {
            delete[] key_;
            delete[] value_;
        }

        template<typename B>
        B dot(const DenseVector<B> &b) const
        {
            B acc;
            for(int j = 0; j < count_; ++j)
              acc += b[key_[j]] * value_[j];
            return acc;
        }

        size_t size() const
        {
            return count_ * (sizeof(int) + sizeof(A));
        }

        static int size(int n)
        {
            return n * (sizeof(int) + sizeof(A));
        }
};

template <typename A>
class CompactVector
{
    private:
        DenseVector<A> *dense_;
        SparseVector<A> *sparse_;

    public:
        CompactVector(const DenseVector<A> &dense, const A &threshold)
        : dense_(0)
        , sparse_(0)
        {
            int sparsity = dense.sparsity(threshold);
            if(SparseVector<A>::size(sparsity) < DenseVector<A>::size(dense.count()))
                sparse_ = new SparseVector<A>(dense, sparsity, threshold);
            else
                dense_ = new DenseVector<A>(dense);
        }

        ~CompactVector()
        {
            if(dense_)
                delete dense_;
            else
                delete sparse_;
        }

        template<typename B>
        B dot(const DenseVector<B> &b) const
        {
            if(dense_)
                return dense_->dot(b);
            else
                return sparse_->dot(b);
        }

        size_t size() const
        {
            if(dense_)
                return dense_->size();
            else
                return sparse_->size();
        }
};

#endif // LINEARALGEBRA_H
