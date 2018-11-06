#pragma once

class cexception : public std::exception
{
public:
    explicit cexception(QString msg) noexcept;
    ~cexception();

    QString msg;
};

