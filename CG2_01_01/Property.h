#pragma once
#include <functional>
#include <assert.h>

enum class AccessorType // Access�^�C�v
{
    ReadOnly    = 0x01,
    WriteOnly   = 0x02,
    AllAccess   = 0x03,
};

template<class T> class Property
{
public:
    Property() = default;
    Property(const Property &other) { ; }
    Property(
        T &r,
        AccessorType type = AccessorType::AllAccess,
        std::function<T()> get = nullptr,
        std::function<void(T value)> set = nullptr):
        r_(r)
    {
        access_type_ = type;
        get_ = get;
        set_ = set;

    }
    // �Q�b�^�[
    operator T() const
    {
        // �������ݐ�p�Ȃ�G���[��Ԃ�
        assert(static_cast<int>(AccessorType::WriteOnly) & static_cast<int>(access_type_));
        // get���ݒ肳��Ă�Γo�^���ꂽ�ق����Ă�
        return get_ ? this->get_() : this->r_;
    }

    // ���ڒ��g���Q�Ƃł���悤�ɃA���[���Z�q���I�[�o�[���[�h
    T *operator ->() const
    {

        return get_ ? &this->get_() : &this->r_;
    }

    // �Z�b�^�[
    void operator =(const T v)
    {
        // �ǂݍ��ݐ�p�Ȃ�G���[��Ԃ�
        assert(static_cast<int>(AccessorType::ReadOnly) & static_cast<int>(access_type_));
        if (set_) // set���ݒ肳��Ă���Γo�^���ꂽ�ق����Ă�
        {
            this->set_(v);
        }
        else
        {
            r_ = v;
        }
    }

private:
    T &r_;                                           // �l
    AccessorType access_type_;                       // �A�N�Z�X�^�C�v
    std::function<T()> get_ = nullptr;               // �Q�b�g���̏���
    std::function<void(T value)> set_ = nullptr;     // �Z�b�g���̏���
};