/*! \file
    \brief Тестим UMBA_RULE_OF_FIVE
 */


//! Часть правила пяти для конструктора по умолчанию (устаревшее определение)
/*!
    UMBA_RULE_OF_FIVE_DEF(className, default|delete);
*/
#define  UMBA_RULE_OF_FIVE_DEF(className, ctorDef)  className () = ctorDef

//! Часть правила пяти для конструктора по умолчанию
/*!
    UMBA_RULE_OF_FIVE_CTOR(className, default|delete);
*/
#define  UMBA_RULE_OF_FIVE_CTOR(className, ctorDef) className () = ctorDef

//! Конструктор по умолчанию - по умолчанию
/*!
    UMBA_RULE_OF_FIVE_CTOR_DEFAULT(className);
*/
#define  UMBA_RULE_OF_FIVE_CTOR_DEFAULT(className)  UMBA_RULE_OF_FIVE_CTOR(className, default)
         
//! Конструктор по умолчанию - запрещён
/*!
    UMBA_RULE_OF_FIVE_CTOR_DELETE(className);
*/
#define  UMBA_RULE_OF_FIVE_CTOR_DELETE(className)   UMBA_RULE_OF_FIVE_CTOR(className, delete)
         



//! Часть правила пяти для конструктора и оператора копирования
/*!
    UMBA_RULE_OF_FIVE_COPY(className, default|delete, default|delete);
*/
#define  UMBA_RULE_OF_FIVE_COPY(className, ctorCopy, opCopy) \
         className (const className &) = ctorCopy;              \
         className & operator= (const className &) = opCopy



//! Часть правила пяти для конструктора и оператора перемещения
/*!
    UMBA_RULE_OF_FIVE_MOVE(className, default|delete, default|delete);
*/
#define  UMBA_RULE_OF_FIVE_MOVE(className, ctorMove, opMove) \
         className (className &&) = ctorMove;                   \
         className & operator=(className &&) = opMove


//! Часть правила пяти для конструктора и оператора копирования и конструктора и оператора перемещения, без конструктора по умолчанию
/*!
    UMBA_RULE_OF_FIVE_COPY_MOVE(className, default|delete, default|delete, default|delete, default|delete);
*/
#define  UMBA_RULE_OF_FIVE_COPY_MOVE(className, ctorCopy, opCopy, ctorMove, opMove) \
         UMBA_RULE_OF_FIVE_COPY(className, ctorCopy, opCopy);                       \
         UMBA_RULE_OF_FIVE_MOVE(className, ctorMove, opMove)



//! Макрос для полного правила пяти
/*!
    UMBA_RULE_OF_FIVE(className, default|delete, default|delete, default|delete, default|delete, default|delete);
*/
#define  UMBA_RULE_OF_FIVE(className, ctorDef, ctorCopy, opCopy, ctorMove, opMove)  \
         UMBA_RULE_OF_FIVE_CTOR(className, ctorDef);                                \
         UMBA_RULE_OF_FIVE_COPY(className, ctorCopy, opCopy);                       \
         UMBA_RULE_OF_FIVE_MOVE(className, ctorMove, opMove)



//! Копирование - по умолчанию
/*!
    UMBA_RULE_OF_FIVE_COPY_DEFAULT(className);
*/
#define  UMBA_RULE_OF_FIVE_COPY_DEFAULT(className) UMBA_RULE_OF_FIVE_COPY(className, default, default)

//! Копирование запрещено
/*!
    UMBA_RULE_OF_FIVE_COPY_DELETE(className);
*/
#define  UMBA_RULE_OF_FIVE_COPY_DELETE(className) UMBA_RULE_OF_FIVE_COPY(className, delete, delete)


//! Перемещение - по умолчанию
/*!
    UMBA_RULE_OF_FIVE_MOVE_DEFAULT(className);
*/
#define  UMBA_RULE_OF_FIVE_MOVE_DEFAULT(className) UMBA_RULE_OF_FIVE_MOVE(className, default, default)

//! Перемещение запрещено
/*!
    UMBA_RULE_OF_FIVE_MOVE_DELETE(className);
*/
#define  UMBA_RULE_OF_FIVE_MOVE_DELETE(className) UMBA_RULE_OF_FIVE_MOVE(className, delete, delete)


//! Копирование и перемещение - по умолчанию
/*!
    UMBA_RULE_OF_FIVE_COPY_MOVE_DEFAULT(className);
*/
#define  UMBA_RULE_OF_FIVE_COPY_MOVE_DEFAULT(className) UMBA_RULE_OF_FIVE_COPY_MOVE(className, default, default, default, default)


//! Копирование и перемещение - запрещено
/*!
    UMBA_RULE_OF_FIVE_COPY_MOVE_DELETE(className);
*/
#define  UMBA_RULE_OF_FIVE_COPY_MOVE_DELETE(className) UMBA_RULE_OF_FIVE_COPY_MOVE(className, delete, delete, delete, delete)


//! Конструктор по умолчанию, копирование и перемещение - по умолчанию
/*!
    UMBA_RULE_OF_FIVE_DEFAULT(className);
*/
#define  UMBA_RULE_OF_FIVE_DEFAULT(className) UMBA_RULE_OF_FIVE(className, default, default, default, default, default)


//! Конструктор по умолчанию, копирование и перемещение - запрещено
/*!
    UMBA_RULE_OF_FIVE_DELETE(className);
*/
#define  UMBA_RULE_OF_FIVE_DELETE(className) UMBA_RULE_OF_FIVE(className, delete, delete, delete, delete, delete)

//
#include <iostream>

using std::cout;
using std::cerr;


struct TestStruct
{
    UMBA_RULE_OF_FIVE(TestStruct, delete, delete, delete, delete, delete);
};




int main(int  /* argc */ , char*  /* argv */ [])
{
    cout << "sizeof(TestStruct): " << sizeof(TestStruct) << "\n";
    return 0;
}