#pragma once
#include <functional>
#include <assert.h>

enum class AccessorType // Accessタイプ
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
    // ゲッター
    operator T() const
    {
        // 書き込み専用ならエラーを返す
        assert(static_cast<int>(AccessorType::WriteOnly) & static_cast<int>(access_type_));
        // getが設定されてれば登録されたほうを呼ぶ
        return get_ ? this->get_() : this->r_;
    }

    // 直接中身を参照できるようにアロー演算子もオーバーロード
    T *operator ->() const
    {

        return get_ ? &this->get_() : &this->r_;
    }

    // セッター
    void operator =(const T v)
    {
        // 読み込み専用ならエラーを返す
        assert(static_cast<int>(AccessorType::ReadOnly) & static_cast<int>(access_type_));
        if (set_) // setが設定されていれば登録されたほうを呼ぶ
        {
            this->set_(v);
        }
        else
        {
            r_ = v;
        }
    }

private:
    T &r_;                                           // 値
    AccessorType access_type_;                       // アクセスタイプ
    std::function<T()> get_ = nullptr;               // ゲット時の処理
    std::function<void(T value)> set_ = nullptr;     // セット時の処理
};