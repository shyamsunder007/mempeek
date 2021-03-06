/*  Copyright (c) 2016-2018, Martin Hammel
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this
      list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
    FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
    SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
    OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef __subroutines_h__
#define __subroutines_h__

#include "mempeek_parser.h"
#include "variables.h"
#include "arrays.h"

#include <string>
#include <map>
#include <set>
#include <vector>
#include <memory>

class ASTNode;


//////////////////////////////////////////////////////////////////////////////
// class SubroutineManager
//////////////////////////////////////////////////////////////////////////////

class SubroutineManager {
public:
    SubroutineManager( Environment* env );
    ~SubroutineManager();

    void begin_subroutine( const yylloc_t& location, std::string name, bool is_function );
    void set_param( std::string name, bool is_array );
    void set_body( std::shared_ptr<ASTNode> body );
    void set_varargs();
    void commit_subroutine();
    void abort_subroutine();

    bool drop_subroutine( std::string name );

    VarManager* get_var_manager();
    ArrayManager* get_array_manager();

    void get_autocompletion( std::set< std::string >& completions, std::string prefix );

    bool has_subroutine( std::string name );
    std::shared_ptr<ASTNode> get_subroutine( const yylloc_t& location, std::string name, const arglist_t& args );

    typedef struct {
        bool is_array;
        union {
            VarManager::var* var;
            ArrayManager::refarray* array;
        } param;
    } param_t;


private:
    typedef struct {
        VarManager* vars;
        ArrayManager* arrays;
        std::vector< param_t > params;
        std::shared_ptr<ASTNode> body;
        VarManager::var* retval = nullptr;
        bool has_varargs;
        yylloc_t location;
    } subroutine_t;

    Environment* m_Environment;

    std::map< std::string, subroutine_t* > m_Subroutines;

    std::string m_PendingName;
    subroutine_t* m_PendingSubroutine = nullptr;
};


//////////////////////////////////////////////////////////////////////////////
// class SubroutineManager inline functions
//////////////////////////////////////////////////////////////////////////////

inline bool SubroutineManager::has_subroutine( std::string name )
{
    auto iter = m_Subroutines.find( name );
    return iter != m_Subroutines.end();
}

inline VarManager* SubroutineManager::get_var_manager()
{
    return m_PendingSubroutine ? m_PendingSubroutine->vars : nullptr;
}

inline ArrayManager* SubroutineManager::get_array_manager()
{
    return m_PendingSubroutine ? m_PendingSubroutine->arrays : nullptr;
}


#endif // __subroutines_h__
