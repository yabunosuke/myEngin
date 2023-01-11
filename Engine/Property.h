#pragma once
// �Q�l�y�[�W
#include <functional>
#include <assert.h>

namespace yEngine
{

    enum class AccessorType // Access�^�C�v
    {
        ReadOnly = 0x01,
        WriteOnly = 0x02,
        AllAccess = 0x03,
    };

    template<class ReferenceT>
    class Property
    {
    public:
        Property() = default;
        //Property(const Property &other) {  };
        Property(
            ReferenceT *r = nullptr,
            AccessorType type = AccessorType::AllAccess,
            std::function<ReferenceT()> get = nullptr,
            std::function<void(ReferenceT value)> set = nullptr) :
            r_(*r),
            access_type_(type),
            get_(get),
            set_(set)
        {
        }
        // �Q�b�^�[
        operator ReferenceT() const
        {
            // �������ݐ�p�Ȃ�G���[��Ԃ�
            //assert(static_cast<int>(AccessorType::WriteOnly) & static_cast<int>(access_type_));
            // get���ݒ肳��Ă�Γo�^���ꂽ�ق����Ă�
            return get_ ? this->get_() : this->r_;
        }

        // ���ڒ��g���Q�Ƃł���悤�ɃA���[���Z�q���I�[�o�[���[�h
        ReferenceT *operator ->() const
        {

            return get_ ? &this->get_() : &this->r_;
        }

        // �Q�Ɠn���I�[�o�[���[�h
        ReferenceT *operator &()const
        {
            return &r_;
        }

        // �Z�b�^�[
        void operator =(const ReferenceT v)
        {
            // �ǂݍ��ݐ�p�Ȃ�G���[��Ԃ�
            assert(static_cast<int>(AccessorType::ReadOnly) != static_cast<int>(access_type_));
            if (set_) // set���ݒ肳��Ă���Γo�^���ꂽ�ق����Ă�
            {
                this->set_(v);
            }
            else
            {
                r_ = v;
            }
        }

        ReferenceT operator -() const
        {
            return get_ ? -this->get_() : -r_;
        }
        ReferenceT operator +(const ReferenceT &v) const
        {
            return get_ ? this->get_() + v : r_ + v;
        }
        ReferenceT &operator +=(const ReferenceT &v) 
        {
            if(set_ != nullptr)
            {
                set_(*this + v);
                return r_;
            }
            return r_ += v;
        }
        ReferenceT operator -(const ReferenceT &v) const
        {
            return get_ ? this->get_() - v : r_ - v;
        }
		ReferenceT &operator -=(const ReferenceT &v) 
        {
            if (set_ != nullptr)
            {
                set_(*this - v);
                return r_;
            }
            return r_ -= v;
        }
        // ��r���Z�q
        bool operator ==(const ReferenceT &v) const
        {
			return get_ ? (this->get_() == v) : (this->r_ == v);
        }
        bool operator !=(const ReferenceT &v) const
        {
            return get_ ? !(this->get_() == v) : !(this->r_ == v);
        }
        bool operator !() const
        {
            return get_ ? !(this->get_()) : !(this->r_);
        }


        ReferenceT &r_;                                           // �l
        AccessorType access_type_;                                // �A�N�Z�X�^�C�v
        std::function<ReferenceT()> get_ = nullptr;               // �Q�b�g���̏���
        std::function<void(ReferenceT value)> set_ = nullptr;     // �Z�b�g���̏���
    };
}